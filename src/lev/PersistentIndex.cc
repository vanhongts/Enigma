/*
 * Copyright (C) 2006 Ronald Lamprecht
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "lev/PersistentIndex.hh"
#include "lev/Proxy.hh"
#include "lev/RatingManager.hh"
#include "errors.hh"
#include "main.hh"
#include "file.hh"
#include "options.hh"
#include "oxyd.hh"
#include "LocalToXML.hh"
#include "utilXML.hh"
#include "Utf8ToXML.hh"
#include "XMLtoUtf8.hh"
#include "ecl_system.hh"

#include <fstream>
#include <iostream>
#include <sstream>
#include <set>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLException.hpp>
#include <xercesc/util/XMLDouble.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>
#include <xercesc/util/XercesVersion.hpp>
#if _XERCES_VERSION < 30000
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#endif

using namespace std;
XERCES_CPP_NAMESPACE_USE 

namespace enigma { namespace lev {
    Variation::Variation(controlType ctrlValue, scoreUnitType unitValue,
            std::string targetValue) : ctrl (ctrlValue),
            unit (unitValue), target (targetValue) {
    }

    bool Variation::operator == (const Variation& otherVar) {
        return ctrl == otherVar.ctrl && unit == otherVar.unit &&
            target == otherVar.target && extensions == otherVar.extensions;
    }
    
    PersistentIndex * PersistentIndex::historyIndex = NULL;
    
    void PersistentIndex::registerPersistentIndices() {
        DirIter * dirIter;
        DirEntry dirEntry;
        
        // SysemPath: register dirs and zips with xml-indices 
        std::vector<std::string> sysPaths = app.systemFS->getPaths();
        std::set<std::string> candidates;
        std::set<std::string> candidates2;
        std::set<std::string> registered;
        for (int i = 0; i < sysPaths.size(); i++) {
            dirIter = DirIter::instance(sysPaths[i] + "/levels");
            while (dirIter->get_next(dirEntry)) {
                if (dirEntry.is_dir && dirEntry.name != "." && dirEntry.name != ".." &&
                        dirEntry.name != ".svn" && dirEntry.name != "cross") {
                    candidates.insert(dirEntry.name);
                }
                else {
                    std::string::size_type zipPos = dirEntry.name.rfind(".zip");
                    if (zipPos != std::string::npos && zipPos == dirEntry.name.size() - 4) {
                        candidates.insert(dirEntry.name.substr(0, dirEntry.name.size() - 4));
                    }
                }
            }
            delete dirIter;
        }

        for (std::set<std::string>::iterator i = candidates.begin(); 
                i != candidates.end(); i++) {
//            Log << "PersistentIndexCandidate1 " << *i <<"\n";
            PersistentIndex * anIndex = new PersistentIndex(*i, true);
            if (!(anIndex->getName().empty())) {
                Index::registerIndex(anIndex);
                registered.insert(*i);
            } else {
                delete anIndex;
            }
        }

        //TODO add system cross indices
        
        
        // UserPath: register dirs and zips with xml-indices excl auto
        dirIter = DirIter::instance(app.userPath + "/levels");
        while (dirIter->get_next(dirEntry)) {
            if (dirEntry.is_dir && dirEntry.name != "." && dirEntry.name != ".." &&
                    dirEntry.name != ".svn" && dirEntry.name != "auto" &&
                    dirEntry.name != "cross" && dirEntry.name != "legacy_dat") {
                if (registered.find(dirEntry.name) == registered.end())
                    candidates2.insert(dirEntry.name);
            }
            else {
                std::string::size_type zipPos = dirEntry.name.rfind(".zip");
                if (zipPos != std::string::npos && zipPos == dirEntry.name.size() - 4) {
                    if (registered.find(dirEntry.name) == registered.end()) 
                        candidates2.insert(dirEntry.name.substr(0, dirEntry.name.size() - 4));
                }
            }
        }
        delete dirIter;

        candidates2.insert("");
        for (std::set<std::string>::iterator i = candidates2.begin(); 
                i != candidates2.end(); i++) {
//            Log << "PersistentIndexCandidate2 " << *i <<"\n";
            PersistentIndex * anIndex = new PersistentIndex(*i, false);
            if (!(anIndex->getName().empty())) {
                Index::registerIndex(anIndex);
                registered.insert(*i);
            } else {
                delete anIndex;
            }
        }
        
       
        //add user cross indices
        dirIter = DirIter::instance(app.userPath + "/levels/cross");
        while (dirIter->get_next(dirEntry)) {
            if (!dirEntry.is_dir && dirEntry.name.size() > 4 && 
                    (dirEntry.name.rfind(".xml") == dirEntry.name.size() - 4)) {
//                Log << "PersistentIndexCandidate4 " << dirEntry.name <<"\n";
                PersistentIndex * anIndex = new PersistentIndex("cross", false, "", dirEntry.name);
                if (!(anIndex->getName().empty())) {
                    Index::registerIndex(anIndex);
                } else {
                    delete anIndex;
                }
            }
        }
        delete dirIter;
         
        // register auto not yet registered new files
        PersistentIndex * autoIndex = new PersistentIndex("auto", false, "Auto");
        autoIndex->indexDefaultLocation = 152000;
        dirIter = DirIter::instance(app.userPath + "/levels/auto");
        while (dirIter->get_next(dirEntry)) { 
            if( !dirEntry.is_dir) {
                if (dirEntry.name.size() > 4 && (
                        (dirEntry.name.rfind(".xml") == dirEntry.name.size() - 4) ||
                        (dirEntry.name.rfind(".lua") == dirEntry.name.size() - 4))) {
                    Proxy * newProxy = Proxy::autoRegisterLevel("auto", 
                            dirEntry.name.substr(0, dirEntry.name.size() - 4));
                    if (newProxy != NULL) {
                        // first check that the proxy is not in the index
                        //  - may occur if the level is stored as .xml and .lua in the folder
                        
                        // is it is new, add it
                        autoIndex->appendProxy(newProxy);
                    }
                }
            }
        }
        delete dirIter;
        Index::registerIndex(autoIndex);
        
        // check if history is available - else generate a new index
        Index * foundHistory = Index::findIndex("History");
        if ( foundHistory != NULL) {
            historyIndex = dynamic_cast<PersistentIndex *>(foundHistory);
        } else {
            historyIndex = new PersistentIndex("cross", false, "History", "history.xml");
            historyIndex->indexDefaultLocation = 153000;
            Index::registerIndex(historyIndex);
        }
    }
    
    void PersistentIndex::addCurrentToHistory() {
        Variation var;
        PersistentIndex * curIndex = dynamic_cast<PersistentIndex *>(Index::getCurrentIndex());
        if (curIndex != NULL)
            var = curIndex->getVariation(curIndex->getCurrentPosition());
        historyIndex->insertProxy(0, Index::getCurrentProxy(), false, var.ctrl, var.unit,
                var.target, var.extensions);
        historyIndex->setCurrentPosition(0);  // last played is always current in history
    }

    PersistentIndex::PersistentIndex(std::string thePackPath, bool systemOnly, 
            std::string anIndexName, std::string theIndexFilename, 
            std::string aGroupName) : 
            Index(anIndexName, aGroupName), packPath (thePackPath), 
            indexFilename(theIndexFilename), isModified (false), doc(NULL) {
//        Log << "PersistentIndex AddLevelPack " << thePackPath << " - " << anIndexName <<  " - " << indexDefaultLocation <<"\n";
        if (thePackPath == "auto")
            return;    // as long as Auto is not editable

        std::auto_ptr<std::istream> isptr;
        ByteVec indexCode;
        std::string errMessage;
        absIndexPath = "";
        std::string relIndexPath = "levels/" + thePackPath + "/" + theIndexFilename;
        if ((!systemOnly && app.resourceFS->findFile(relIndexPath, absIndexPath, isptr)) ||
                (systemOnly && app.systemFS->findFile(relIndexPath, absIndexPath, isptr))) {
            // preload index file or zipped index
            if (isptr.get() != NULL) {
                // zipped file
                Readfile (*isptr, indexCode);
            } else {
                // plain file
                std::basic_ifstream<char> ifs(absIndexPath.c_str(), ios::binary | ios::in);
                Readfile(ifs, indexCode);
            }
            try {
                std::ostringstream errStream;
                app.domParserErrorHandler->resetErrors();
                app.domParserErrorHandler->reportToOstream(&errStream);
                app.domParserSchemaResolver->resetResolver();
                app.domParserSchemaResolver->addSchemaId("index.xsd","index.xsd");
                // preloaded  xml or zipped xml
                std::auto_ptr<Wrapper4InputSource> domInputIndexSource ( new Wrapper4InputSource(
                        new MemBufInputSource(reinterpret_cast<const XMLByte *>(&(indexCode[0])),
                        indexCode.size(), absIndexPath.c_str(), false)));
                doc = app.domParser->parse(*domInputIndexSource);

                if (doc != NULL && !app.domParserErrorHandler->getSawErrors()) {
                    infoElem = dynamic_cast<DOMElement *>(doc->getElementsByTagName(
                            Utf8ToXML("info").x_str())->item(0));
                    levelsElem = dynamic_cast<DOMElement *>(doc->getElementsByTagName(
                            Utf8ToXML("levels").x_str())->item(0));
                }

                if(app.domParserErrorHandler->getSawErrors()) {
                    errMessage = errStream.str();
                }
                app.domParserErrorHandler->reportToNull();  // do not report to errStream any more
            }
            catch (...) {
                errMessage = "Unexpected XML Exception on load of index\n";
            }
            if (!errMessage.empty()) {
                if (doc != NULL) {
                    doc->release();           // empty or errornous doc 
                    doc = NULL;
                }
                Log << errMessage;   // make long error messages readable
                return;
            } else if (doc != NULL) {
                //TODO check if an updated index exists for system packs
                indexName = XMLtoUtf8(infoElem->getAttribute( 
                        Utf8ToXML("title").x_str())).c_str();                
                indexGroup = XMLtoUtf8(infoElem->getAttribute( 
                        Utf8ToXML("group").x_str())).c_str();                
                owner = XMLtoUtf8(infoElem->getAttribute( 
                        Utf8ToXML("owner").x_str())).c_str();                
                XMLDouble * result = new XMLDouble(infoElem->getAttribute( 
                        Utf8ToXML("location").x_str()));
                indexDefaultLocation = result->getValue();
                DOMNodeList *levelList = levelsElem->getElementsByTagName(
                        Utf8ToXML("level").x_str());
                std::set<std::string> knownAttributes;
                knownAttributes.insert("_seq");
                knownAttributes.insert("_title");
                knownAttributes.insert("_xpath");
                knownAttributes.insert("id");
                knownAttributes.insert("author");
                knownAttributes.insert("score");
                knownAttributes.insert("rel");
                knownAttributes.insert("rev");
                knownAttributes.insert("easy");
                knownAttributes.insert("ctrl");
                knownAttributes.insert("unit");
                knownAttributes.insert("target");
                for (int i = 0, l = levelList->getLength();  i < l; i++) {
                    DOMElement *levelElem = dynamic_cast<DOMElement *>(levelList->item(i));
                    std::string path = XMLtoUtf8(levelElem->getAttribute( 
                            Utf8ToXML("_xpath").x_str())).c_str();
                    std::string id = XMLtoUtf8(levelElem->getAttribute( 
                            Utf8ToXML("id").x_str())).c_str();
                    std::string title = XMLtoUtf8(levelElem->getAttribute( 
                            Utf8ToXML("_title").x_str())).c_str();
                    std::string author = XMLtoUtf8(levelElem->getAttribute( 
                            Utf8ToXML("author").x_str())).c_str();
                    int scoreVersion = XMLString::parseInt(levelElem->getAttribute( 
                            Utf8ToXML("score").x_str()));
                    int releaseVersion = XMLString::parseInt(levelElem->getAttribute( 
                            Utf8ToXML("rel").x_str()));
                    int revisionVersion = XMLString::parseInt(levelElem->getAttribute( 
                            Utf8ToXML("rev").x_str()));
                    bool hasEasymodeFlag = boolValue(levelElem->getAttribute( 
                            Utf8ToXML("easy").x_str()));
                    Proxy * newProxy = Proxy::registerLevel(path, thePackPath, id, title,
                            author, scoreVersion, releaseVersion, hasEasymodeFlag, 
                            GAMET_ENIGMA, STATUS_RELEASED, revisionVersion);
                    Variation var;
                    std::string controlString = XMLtoUtf8(levelElem->getAttribute( 
                            Utf8ToXML("ctrl").x_str())).c_str();
                    if (controlString == "balance")
                        var.ctrl = balance;
                    else if  (controlString == "key")
                        var.ctrl = key;
                    else if  (controlString == "other")
                        var.ctrl = other;
                    std::string txt = XMLtoUtf8(levelElem->getAttribute( 
                        Utf8ToXML("unit").x_str())).c_str();
                    if (txt == "number")
                        var.unit = number;
                    else
                        // default
                        var.unit = duration;
                    var.target = XMLtoUtf8(levelElem->getAttribute( 
                            Utf8ToXML("target").x_str())).c_str();
                    DOMNamedNodeMap * attrMap = levelElem->getAttributes();
                    for (int j = 0, k = attrMap->getLength();  j < k; j++) {
                        DOMAttr * levelAttr = dynamic_cast<DOMAttr *>(attrMap->item(j));
                        std::string attrName = XMLtoUtf8(levelAttr->getName()).c_str();
                        if (knownAttributes.find(attrName) == knownAttributes.end()) {
                            Log << "PersistentIndex Load unknown Attribut: " << attrName << "\n";
                            var.extensions[attrName]= XMLtoUtf8(levelAttr->getValue()).c_str();
                        }
                        
                    }
                    appendProxy(newProxy, var.ctrl, var.unit, var.target, var.extensions);                 
                }
            }
        }        
    }
        
    PersistentIndex::~PersistentIndex() {
       if (doc != NULL)
           doc->release();
    }
    
    void PersistentIndex::clear() {
        proxies.clear();
    }
    
    void PersistentIndex::appendProxy(Proxy * newLevel, controlType varCtrl,
                scoreUnitType varUnit, std::string varTarget,
                std::map<std::string, std::string> varExtensions) {
        proxies.push_back(newLevel);
        
        Variation var(varCtrl, varUnit, varTarget);
        var.extensions = varExtensions;
        variations.push_back(var);
    }

    void PersistentIndex::insertProxy(int pos, Proxy * newLevel, bool allowDuplicates,
                controlType varCtrl, scoreUnitType varUnit, std::string varTarget,
                std::map<std::string, std::string> varExtensions) {
        // TODO Assert pos >= size
        
        Variation var(varCtrl, varUnit, varTarget);
        var.extensions = varExtensions;
        
        std::vector<Proxy *>::iterator itProxy = proxies.begin();
        std::vector<Variation>::iterator itVar = variations.begin();
        if (!allowDuplicates) {
            // delete duplicates
            while (itProxy != proxies.end()) {
                if (*itProxy == newLevel && *itVar == var) {
                    Log << "History Duplicat found\n";
                    itProxy = proxies.erase(itProxy);
                    itVar = variations.erase(itVar);
                }
                if (itProxy != proxies.end())
                    itProxy++; itVar++;
            }
        }
        
        itProxy = proxies.begin();
        itVar = variations.begin();
        for (int i = 0; i < pos; i++) {
                itProxy++; itVar++;
        }
        proxies.insert(itProxy, newLevel);
        variations.insert(itVar, var);
    }
    
    Variation PersistentIndex::getVariation(int pos) {
        // TODO Assert pos
        return variations[pos];
    }

    void PersistentIndex::deletesave() {
        std::vector<Proxy *>::iterator it = proxies.begin();
        it++; it++; it++;
        proxies.erase(it);
        std::vector<Variation>::iterator itv = variations.begin();
        itv++; itv++; itv++;
        variations.erase(itv);
        save();
    }
    
    bool PersistentIndex::save() {
        bool result = true;
        
        if (doc == NULL) {
            std::string errMessage;
            std::string indexTemplatePath;
            if (app.systemFS->findFile( "schemas/index.xml" , indexTemplatePath)) {
                try {
                    std::ostringstream errStream;
                    app.domParserErrorHandler->resetErrors();
                    app.domParserErrorHandler->reportToOstream(&errStream);
                    app.domParserSchemaResolver->resetResolver();
                    app.domParserSchemaResolver->addSchemaId("index.xsd","index.xsd");
                    doc = app.domParser->parseURI(indexTemplatePath.c_str());
                    if (doc != NULL && !app.domParserErrorHandler->getSawErrors()) {
                        infoElem = dynamic_cast<DOMElement *>(doc->getElementsByTagName(
                                Utf8ToXML("info").x_str())->item(0));
                        levelsElem = dynamic_cast<DOMElement *>(doc->getElementsByTagName(
                                Utf8ToXML("levels").x_str())->item(0));
                    }
                     if(app.domParserErrorHandler->getSawErrors()) {
                        errMessage = errStream.str();
                    }
                    app.domParserErrorHandler->reportToNull();  // do not report to errStream any more
                }
                catch (...) {
                    errMessage = "Unexpected XML Exception on load of index\n";
                }
                if (!errMessage.empty()) {
                    if (doc != NULL) {
                        doc->release();           // empty or errornous doc 
                        doc == NULL;
                    }
                    Log << errMessage;   // make long error messages readable
                    return false;
                }
            } else
                // TODO add error handling
                return false;
        }
        
        // 
        
        infoElem->setAttribute( Utf8ToXML("title").x_str(), 
                Utf8ToXML(&indexName).x_str());
        infoElem->setAttribute( Utf8ToXML("group").x_str(), 
                Utf8ToXML(INDEX_DEFAULT_GROUP).x_str());
        infoElem->setAttribute( Utf8ToXML("location").x_str(),
                Utf8ToXML(ecl::strf("%g",indexDefaultLocation)).x_str());
        DOMNodeList *levelsChildList = levelsElem->getChildNodes();
        int levelsChildCount = levelsChildList->getLength();
        // delete no more used level elements
        for (int i = 0; i < levelsChildCount; i++) {
            // delete all elements of list, as the list is dynamically updated -
            // we can not recycle level elements as they may be reordered and
            // their attributes are not identical
            levelsElem->removeChild(levelsChildList->item(0));
        }
        DOMElement * levelElem;
        // add level elements
        for (int i = 0; i < size(); i++) {
            // add a new element
            levelElem = doc->createElement(Utf8ToXML("level").x_str());
            levelsElem->appendChild(levelElem);   // insert it at the end
            
            Proxy * level = getProxy(i);
            // convert Proxy normLevelPath to pack local path ./* if possible
            std::string xpath = level->getNormLevelPath();
            if (xpath.find(packPath + "/") == 0)
                xpath = "." + xpath.substr(packPath.size());
            else if (packPath.empty() && xpath.find("/") == std::string::npos)
                xpath = "./" + xpath;
            levelElem->setAttribute( Utf8ToXML("_xpath").x_str(),
                    Utf8ToXML(xpath).x_str());
            levelElem->setAttribute( Utf8ToXML("id").x_str(),
                    Utf8ToXML(level->getId()).x_str());
            levelElem->setAttribute( Utf8ToXML("_title").x_str(),
                    Utf8ToXML(level->getTitle()).x_str());
            levelElem->setAttribute( Utf8ToXML("author").x_str(),
                    Utf8ToXML(level->getAuthor()).x_str());
            levelElem->setAttribute( Utf8ToXML("score").x_str(),
                    Utf8ToXML(ecl::strf("%d",level->getScoreVersion())).x_str());
            levelElem->setAttribute( Utf8ToXML("rel").x_str(),
                    Utf8ToXML(ecl::strf("%d",level->getReleaseVersion())).x_str());
            levelElem->setAttribute( Utf8ToXML("rev").x_str(),
                    Utf8ToXML(ecl::strf("%d",level->getRevisionNumber())).x_str());
            levelElem->setAttribute( Utf8ToXML("easy").x_str(),
                    Utf8ToXML(level->hasEasymode() ? "true" : "false").x_str());
            std::string control;
            switch (variations[i].ctrl) {
            case lev::force:
                control = "force"; break;
            case lev::balance:
                control = "balance"; break;
            case lev::key:
                control = "key"; break;
            default:
                control = "other"; break;
            }
            levelElem->setAttribute( Utf8ToXML("ctrl").x_str(),
                    Utf8ToXML(control).x_str());
            std::string unit;
            switch (variations[i].unit) {
            case lev::duration:
                unit = "duration"; break;
            case lev::number:
                unit = "number"; break;
            }
            levelElem->setAttribute( Utf8ToXML("unit").x_str(),
                    Utf8ToXML(unit).x_str());
            levelElem->setAttribute( Utf8ToXML("target").x_str(),
                    Utf8ToXML(variations[i].target).x_str());
            for (std::map<std::string, std::string>::iterator j= variations[i].extensions.begin(); 
                    j != variations[i].extensions.end(); j++) {
//                Log << "Persistent save extension: " << (*j).first << " - " << (*j).second << "\n";
                levelElem->setAttribute( Utf8ToXML((*j).first).x_str(),
                        Utf8ToXML((*j).second).x_str());
            }
        }       
        
        // update the sequence number of the levels
        DOMNodeList *levList = levelsElem->getElementsByTagName(
                Utf8ToXML("level").x_str());
        for (int i = 0, l = levList-> getLength();  i < l; i++) {
            DOMElement *levElem = dynamic_cast<DOMElement *>(levList->item(i));
            levElem->setAttribute( Utf8ToXML("_seq").x_str(),
                    Utf8ToXML(ecl::strf("%d",i+1)).x_str());           
        }
        
        stripIgnorableWhitespace(doc->getDocumentElement());

        std::string path = app.userPath + "/levels/" + packPath + "/" + indexFilename;
        try {
            // auto-create the directory if necessary
            std::string directory;
            if (ecl::split_path (path, &directory, 0) && !ecl::FolderExists(directory)) {
                ecl::FolderCreate (directory);
            }

#if _XERCES_VERSION >= 30000
            result = app.domSer->writeToURI(doc, LocalToXML(& path).x_str());
#else
            XMLFormatTarget *myFormTarget = new LocalFileFormatTarget(path.c_str());
            result = app.domSer->writeNode(myFormTarget, *doc);
            delete myFormTarget;   // flush
#endif
        }
        catch (const XMLException& toCatch) {
            char* message = XMLString::transcode(toCatch.getMessage());
            cerr << "Exception on save of index: "
                 << message << "\n";
            XMLString::release(&message);
            result = false;
        }
        catch (const DOMException& toCatch) {
            char* message = XMLString::transcode(toCatch.msg);
            cerr << "Exception on save of index: "
                 << message << "\n";
            XMLString::release(&message);
            result = false;
        }
        catch (...) {
            cerr << "Unexpected exception on save of index\n" ;
            result = false;
        }
        if (!result)
            Log << "Index save fault on " << path << " \n";
        else
            Log << "Index save " << path << " o.k.\n";
        
        return result;
    }


    PersistentIndex::PersistentIndex(std::istream *legacyIndexStream, 
            std::string thePackPath, bool isZip, std::string anIndexName, 
            std::string theIndexFilename) : 
            Index(anIndexName, INDEX_DEFAULT_GROUP), 
            indexFilename(theIndexFilename), isModified (false), doc(NULL) {
        Log << "PersistentIndex convert 0.92 index " << thePackPath << " - " << anIndexName <<"\n";
        lev::RatingManager *theRatingMgr = lev::RatingManager::instance();

        // prepare Proxy coding of pack path -        
        if (thePackPath == "levels")
            packPath = "";
        else {
            //  cut off leading "levels/" 
            packPath = thePackPath.substr(7);
        }

        int linenumber = 0;
        try {
            std::string line;
            while (std::getline(*legacyIndexStream, line)) {
                using namespace std;
                using namespace ecl;
    
                string   filename = "";              //< Filename of the level (exl. extension)
                string   indexname = "";             //< The name used in the options file, 
                                                //   empty string -> use filename entry
                string   name = "";                  //< Complete name of the level
                string   author = "";                //< Author of the level
                int      revision = 1;              //< Revision # of this level
                bool     has_easymode = false;          //< whether level has an easymode
                int      par_time_easy = -1 ;        //< Best time in seconds (for easy mode)
                int      par_time_normal = -1;      //< Best time in seconds (for normal mode)
                string   par_time_easy_by = "";     //< player name(s) for 'best_time_easy'
                string   par_time_normal_by = "";   //< same for 'best_time_normal'
                int      par_moves = -1;            //< Minimum moves to solve level
                int      intelligence = 0;
                int      dexterity = 0;
                int      patience = 0;
                int      knowledge = 0 ;
                int      speed = 0;
 
                ++linenumber;
   
                const char *wspace = " \t";
                size_t      p      = line.find_first_not_of(wspace);
        
                if (p == string::npos || line.at(p) != '{')
                    continue;
        
                // found a level description
        
                int    par_time        = -1;
                string par_time_by;
                string par_moves_by;
        
                ++p;
                while (true) {
                    string tag;
                    string content;
        
                    // ugly parser code starts here.
                    // - it parses 'tag = content' or 'tag = "content"'
                    // - '\"' is allowed in '"content"'
                    // - whitespace is ignored
                    // - sets message and breaks while loop in case of error
                    size_t tag_start = line.find_first_not_of(wspace, p);
                    Assert <XLevelPackInit> (tag_start != string::npos, "Expected tag or '}'");
                    if (line.at(tag_start) == '}') break; // line done
        
                    size_t equal = line.find('=', tag_start);
                    Assert <XLevelPackInit> (equal != string::npos, "'=' expected");
                    Assert <XLevelPackInit> (equal != tag_start, "empty tag");
        
                    size_t tag_end = line.find_last_not_of(wspace, equal-1);
                    tag = line.substr(tag_start, tag_end-tag_start+1);
        
                    size_t content_start = line.find_first_not_of(wspace, equal+1);
                    if (line.at(content_start) == '\"') { // content in ""
                        size_t oquote       = line.find('\"', content_start+1);
                        bool   have_escapes = false;
                        while (oquote != string::npos && line.at(oquote-1) == '\\') { // step over \"
                            oquote       = line.find('\"', oquote+1);
                            have_escapes = true;
                        }
                        Assert <XLevelPackInit> (oquote != string::npos, "unmatched quote");
                        content = line.substr(content_start+1, oquote-content_start-1);
                        if (have_escapes) {
                            size_t esc;
                            while ((esc = content.find("\\\"")) != string::npos)
                                content.replace(esc, 2, "\"");
                        }
                        p = oquote+1;
                    }
                    else { // content w/o ""
                        size_t content_end = line.find_first_of(" \t}", content_start);
                        Assert <XLevelPackInit> (content_end != string::npos, "expected space or } behind content");
                        content = line.substr(content_start, content_end-content_start);
                        p = content_end;
                    }
        
                    if      (tag == "file")     filename     = content;
                    else if (tag == "indexname")indexname    = content;
                    else if (tag == "name")     name         = content;
                    else if (tag == "author")   author       = content;
                    else if (tag == "revision") revision     = atoi(content.c_str());
                    else if (tag == "easymode") has_easymode = (content == "1");
                    else if (tag == "int")      intelligence = atoi(content.c_str());
                    else if (tag == "dex")      dexterity    = atoi(content.c_str());
                    else if (tag == "pat")      patience     = atoi(content.c_str());
                    else if (tag == "kno")      knowledge    = atoi(content.c_str());
                    else if (tag == "spe")      speed        = atoi(content.c_str());
                    else if (tag == "par_time")        parsePar(content, par_time, par_time_by);
                    else if (tag == "par_time_easy")   parsePar(content, par_time_easy, par_time_easy_by);
                    else if (tag == "par_time_normal") parsePar(content, par_time_normal, par_time_normal_by);
                    else if (tag == "par_moves")       parsePar(content, par_moves, par_moves_by);
        //                 else if (tag == "hint1") hint1           = content;
        //                 else if (tag == "hint2") hint2           = content;
                    else
                        throw XLevelPackInit(strf("unknown tag '%s'", tag.c_str()));
                }
        
                Assert <XLevelPackInit> (filename.length() != 0, 
                                         "mandatory tag 'file' missing");
        
                if (has_easymode) {
                    Assert <XLevelPackInit> (par_time == -1,
                                             "'par_time' not allowed when easymode=1 "
                                             "(use 'par_time_easy' and 'par_time_normal')");
                }
                else {
                    Assert <XLevelPackInit> (par_time_normal==-1 && par_time_easy==-1,
                            "'par_time_easy' and 'par_time_normal' are not allowed when easymode=0 (use 'par_time')");
                    par_time_easy    = par_time_normal    = par_time;
                    par_time_easy_by = par_time_normal_by = par_time_by;
                }
                
                // correct filename for zip 
                if (isZip) {
                    Log << "Zip " << thePackPath << " - " <<packPath << " - " << indexname << "\n";
                    if (filename.find('/') == std::string::npos) {
                        // it is an old zip file with plain levelnames as filename
                        if (indexname.empty()) {
                            // keep id
                            indexname = filename;
                        }
                        // add the zip basename to get the complete path to the level
                        filename = packPath + "/" + filename;
                    }
                } else if (linenumber == 1 && packPath.empty()) {
                    // a std index.txt on levels dir - we need to guess location of pack
                    // we take the dir of the first level
                    std::string::size_type n = filename.rfind('/');
                    if (n != std::string::npos) {
                        packPath = filename.substr(0, n);
//                        Log << "Index convert pack '" << anIndexName << "' - guess pack directory is '" 
//                                << packPath << "'\n";
                    }
                }
                
                // register Proxy
                appendProxy(Proxy::registerLevel(filename, packPath,
                        (indexname.empty() ? filename : indexname), name, author, 
                        revision, revision, has_easymode, GAMET_ENIGMA));
                theRatingMgr->registerRating((indexname.empty() ? filename : indexname),
                    revision, intelligence, dexterity, patience, knowledge, speed, 
                    par_time_easy, par_time_easy_by, par_time_normal, par_time_normal_by);
                    
            }
        } catch (const XLevelPackInit &e) {
            std::string xerror = ecl::strf("in line %i: %s", linenumber, e.what());
            throw XLevelPackInit (xerror);
        }
        
        // convert to XML
        updateFromProxies();
        save();
    }

    void PersistentIndex::parsePar(const string& par, int& par_value, std::string& par_text) 
    {
        // 'par' is in format "value,text"
        // The value is stored in 'par_value' and the text in 'par_text'.
        using namespace std;
        using namespace ecl;

        size_t comma = par.find(',');
        Assert<XLevelPackInit> (comma!=string::npos, "Comma expected in par");

        string value = par.substr(0, comma);
        par_text     = par.substr(comma+1);
        par_value    = atoi(value.c_str());
    }

    void AddLevelPack (const char *init_file, const char *indexName) {
//        Log << "Index AddLevelPack " << init_file << "\n";
        if (Index::findIndex(indexName) == NULL) {
            std::string absPath;
            if (app.resourceFS->findFile(init_file, absPath)) {
                try {
                    std::string path = init_file;
                    std::string dir = "";
                    std::string filename = "";
                    ecl::split_path(path, &dir, &filename);
                    std::ifstream is(absPath.c_str());
                
                    if (!is)
                        throw XLevelPackInit ("Cannot open index file");
                    Index::registerIndex(new PersistentIndex(&is, dir, false, indexName));
                } catch (const XLevelPackInit &e) {
                    Log << e.get_string() << "\n";
                }
            } else {
                Log << "Could not find level index file `" << init_file << "'\n";
            }
        } else {
//            Log << "Ignored index file `" << init_file << "' - already converted to XML\n";
        }
    }

    void AddZippedLevelPack (const char *zipfile) {
//        Log << "Index AddZippedLevelPack " << zipfile << "\n";
        using namespace std;
        using namespace ecl;
        string absPath;
        if (app.resourceFS->findFile (zipfile, absPath)) {
            // the index file as it would be for a unpacked zip
            std::string zf = zipfile;
            std::string dir = zf.substr(0, zf.rfind('.'));
            std::string indexfile = dir + "/index.txt";
            try {
                auto_ptr<istream> isptr;
                std::string dummy;
                if(!app.resourceFS->findFile(indexfile, dummy, isptr))
                    throw XLevelPackInit ("No index in level pack: ");
                
                istream &is = *isptr;
        
                string line;
                std::string indexName;
                if (getline(is, line)) {
                    // we read the index in binary mode and have to strip of the \n for
                    // windows
                    if (line[line.size()-1] = '\n') {
                        line.resize(line.size()-1);
                    }
                    indexName = line;
                    
                    // check if already loaded
                    
                    Index::registerIndex(new PersistentIndex(isptr.get(), dir, true, indexName));
                }
                else {
                    throw XLevelPackInit ("Invalid level pack: " + indexName);
                }
            }
            catch (std::exception &) {
                throw XLevelPackInit ("Error reading from .zip file: " + indexfile);
            }
        } else {
            enigma::Log << "Could not find zip file `" << zipfile << "'\n";
        }
    }

}} // namespace enigma::lev
