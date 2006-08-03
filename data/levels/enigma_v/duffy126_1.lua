-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Elaborate

rooms_wide=4
rooms_high=5

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)
enigma.SlopeForce=70

oxyd_default_flavor="a"

fill_floor("fl-leaves", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c =="0" then
			set_floor("fl-ice_001",i-1,line)
			set_item("it-pin",i-1,line)
		elseif c =="s" then
			set_stone( "st-greenbrown", i-1, line)
		   set_floor("fl-inverse",i-1,line)
		elseif c =="G" then
	   set_stone( "st-fakeoxyda", i-1, line)
		elseif c =="|" then
	   set_stone( "st-fakeoxyda", i-1, line)
	   set_item("it-cherry",i-1,line)
		elseif c =="e" then
		   set_stone("st-glass2_hole",i-1,line)
  		   set_item("it-vortex-open", i-1, line, {targetx="22.5",targety="52.5"})
		elseif c =="I" then
  		   set_item("it-vortex-open", i-1, line, {targetx="1.5",targety="54.5"})
		elseif c =="," then
  		   set_item("it-vortex-open", i-1, line)
		elseif c =="?" then
			set_stone( "st-greenbrown", i-1, line)
			set_floor("fl-ice_001",i-1,line)
		elseif c =="x" then
			set_stone( "st-greenbrown", i-1, line)
			set_floor("fl-swamp",i-1,line)
		elseif c =="X" then
			set_stone( "st-death", i-1, line)
		elseif c =="f" then
			set_stone( "st-death_invisible", i-1, line)
		elseif c =="v" then
			set_stone( "st-glass", i-1, line)
		elseif c =="V" then
			set_stone( "st-brick_magic", i-1, line)
		elseif c =="`" then
		   set_stone( "st-bigbrick-nw", i-1, line)
		elseif c =="@" then
		   set_stone( "st-bigbrick-ne", i-1, line)
		elseif c =="^" then
		   set_stone( "st-bigbrick-es", i-1, line)
		elseif c =="-" then
		   set_stone( "st-bigbrick-sw", i-1, line)
		elseif c =="8" then
		   set_stone( "st-bigbrick-n", i-1, line)
		elseif c =="6" then
		   set_stone( "st-bigbrick-e", i-1, line)
		elseif c =="." then
		   set_stone( "st-bigbrick-s", i-1, line)
		elseif c ==";" then
		   set_stone( "st-bigbrick-w", i-1, line)
		elseif c ==":" then
		   set_stone( "st-bigbrick-nesw", i-1, line)
		elseif c =="t" then
			set_stone( "st-brick_magic", i-1, line)
			set_item("it-surprise",i-1,line)
		elseif c =="/" then
			set_stone( "st-brick_magic", i-1, line)
			set_item("it-extralife",i-1,line)
		elseif c =="O" then
			set_item("it-extralife",i-1,line)
		elseif c =="$" then
			set_stone( "st-greenbrown", i-1, line)
			set_floor("fl-sand",i-1,line)
		elseif c =="%" then
			set_stone( "st-greenbrown", i-1, line)
			set_floor("fl-normal",i-1,line)
		elseif c =="&" then
			set_stone( "st-greenbrown", i-1, line)
			set_floor("fl-hay",i-1,line)
		elseif c =="F" then
			set_stone( "st-flash", i-1, line)
			set_floor("fl-ice_001",i-1,line)
		elseif c =="a" then
			set_stone( "st-actorimpulse", i-1, line)
			set_floor("fl-ice_001",i-1,line)
		elseif c =="Y" then
			set_stone( "st-yinyang1", i-1, line)
			set_floor("fl-ice_001",i-1,line)
		elseif c =="S" then
			set_stone( "st-surprise", i-1, line)
		elseif c =="L" then
			set_stone( "st-window", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "J" then
			oxyd( i-1, line)
			set_floor("fl-hay",i-1,line)
		elseif c == "<" then
			oxyd( i-1, line)
			set_floor("fl-normal",i-1,line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "~" then
		   set_floor("fl-water",i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-1,line+.5, {essential=1,player=0})
		elseif c=="y" then
		   set_actor("ac-whiteball", i-.5,line+.5, {player=1})
		elseif c=="w" then
		   set_actor("ac-killerball", i-.5,line+.5, {player=1,mouseforce=0})
			set_floor("fl-ice_001",i-1,line)
		elseif c=="R" then
		   set_actor("ac-rotor", i-.5,line+.5, {force=30,range=8})
		elseif c=="N" then
		   set_actor("ac-top", i-1,line+.5, {force=40,range=9})
		   set_item("it-cherry",i-1,line)
		elseif c==">" then
		   set_item("it-cherry",i-1,line)
		elseif c=="b" then
		   set_actor("ac-bug", i-.5,line+.5)
			set_floor("fl-sand",i-1,line)
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
			set_floor("fl-space",i-1,line-1)
			set_floor("fl-space",i-1,line+1)
		elseif c=="i" then
			set_floor("fl-ice_001",i-1,line)
		elseif c=="n" then
			set_floor("fl-normal",i-1,line)
		elseif c=="j" then
			set_floor("fl-hay",i-1,line)
		elseif c=="q" then
		   set_floor("fl-sand",i-1,line)
		elseif c=="m" then
		   set_floor("fl-swamp",i-1,line)
		elseif c=="Q" then
		   set_floor("fl-inverse",i-1,line)
		elseif c=="p" then
		   set_floor("fl-swamp",i-1,line)
		   set_item("it-extralife",i-1,line)
		elseif c=="_" then
			fill_floor("fl-brick",i-1,line,18,11)
		elseif c=="Z" then
			fill_floor("fl-himalaya",i-2,line,19,12)
		elseif c=="H" then
			set_stone("st-rock3_break", i-1, line)
		elseif c=="T" then
			set_stone("st-rock3_break", i-1, line)
			set_item("it-surprise",i-1,line)
		elseif c=="h" then
			set_stone("st-rock3", i-1, line)
		elseif c=="P" then
			set_item("it-hammer",i-1,line)
		elseif c=="M" then
			set_stone( "st-brownie", i-1, line)
			set_item("it-magicwand",i-1,line)
		elseif c=="D" then
			set_item("it-dynamite",i-1,line)
		elseif c=="B" then
			set_stone("st-bug", i-1, line)
		elseif c=="c" then
			set_stone("st-bug", i-1, line)
			set_item("it-floppy",i-1,line)
		elseif c == "d" then --1-d
			set_floor("fl-gradient",  i-1,  line, {type=1})
		elseif c == "u" then --2-u
			set_floor("fl-gradient",  i-1,  line, {type=2})
		elseif c == "r" then --3-r
			set_floor("fl-gradient",  i-1,  line, {type=3})
		elseif c == "l" then --4-l
			set_floor("fl-gradient",  i-1,  line, {type=4})
		elseif c == "1" then --ur
			set_floor("fl-gradient",  i-1,  line, {type=11})
		elseif c == "3" then --dl
			set_floor("fl-gradient",  i-1,  line, {type=9})
		elseif c == "7" then --dr
			set_floor("fl-gradient",  i-1,  line, {type=12})
		elseif c == "9" then --ul
			set_floor("fl-gradient",  i-1,  line, {type=10})
		elseif c=="U" then
			set_stone("st-bolder", i-1,line, {direction=NORTH})
		   set_floor("fl-water",i-1,line)
		elseif c=="4" then
			yy1( "white",  i-1, line)
			set_floor("fl-gradient",  i-1,  line, {type=4})
		elseif c=="5" then
			yy1( "white",  i-1, line)
			set_floor("fl-gradient",  i-1,  line, {type=11})
		elseif c=="2" then
			yy1( "white",  i-1, line)
			set_floor("fl-gradient",  i-1,  line, {type=1})
		elseif c=="W" then
			yy1( "black",  i-1, line)
		elseif c=="C" then
			set_stone( "st-floppy", i-1,line,{action="openclose",target="door1"})
			set_floor("fl-hay",i-1,line)
		elseif c=="A" then
			doorv( i-1,line, {name="door1"})
			set_floor("fl-hay",i-1,line)
		elseif c=="K" then
            set_item("it-wormhole", i-1, line, {strength=0,targetx="49.5",targety="49.5"})
		elseif c=="k" then
			set_attrib(laser(i-1,line, TRUE, NORTH), "name", "laser1")
		elseif c=="E" then
		   document(i-1,line,"It is not quite that easy.")
	     end
	end	
     end

function yy1( color, x, y)
        stone = format( "st-%s4", color)
        set_stone( stone, x, y)
end

renderLine(00,"#######################################??????????????????#hhhhhhhhhhhhhhhhhhh")
renderLine(01,"#cBB        #    31#                  #iiiaiiiiiiiiaiiiii#   =====          h")
renderLine(02,"#BoB        #    97#        ~~~~      #iiiiiiiiaiiiiiiiia#   =====        o h")
renderLine(03,"#BBB        # 31   #      ~~~~~~~     #iiaiiiiiiiiaiiiaii#   =====          h")
renderLine(04,"#           #y97  o#     ~~~~~~~~~    #aiiiiiiiaiiiiiiiii#   =====          h")
renderLine(05,"#           ########    ~~~~~~~~~~~   #iiiiaiiiiiiiiiiaii#   =====          h")
renderLine(06,"#       F          W    ~~~~~~~~~~~~   iiiiiiaiiiiiaiiiii    =====    D     h")
renderLine(07,"#                  #   ~~~~~~+ ~~~~~  #iiiiiiiiiaiiiiiiii#   =====          h")
renderLine(08,"#                  #  ~~~~~~ + ~~~~~~ #iiiaiii0iiiiiiiaii#   =====          h")
renderLine(09,"#                  #  ~~~~~~+ +~~~~~~~#iiiiiaiiaiiiaiiiii#   =====          h")
renderLine(10,"#                  # ~~~~~~ + +  ~~~~~#iaiiiiiiiiiiiiiaii#   =====      O   h")
renderLine(11,"#                  #o~~~~~~      ~~~~~#iiiiaiiiiiaiiiiiia#   =====          h")
renderLine(12,"hHHHHHhhhhhhhhhhhhh#########  #########$$$$$$$$$$$$$$$$%%%HHHHHHHHHHHHHHHHHHh")
renderLine(13,"h                  #       #  #       #qqqqqqqqq31qqqqqrn%                  #")
renderLine(14,"h                  #                  #qqqqq3d1q97qqqqq9u%                  #")
renderLine(15,"h                  #   SSSSSSSSSSSS   #qqqqqrnlqqqqqqqqqq$                  #")
renderLine(16,"h                  #   SSSSSSSSSSSS   #qqqqq9u7qqq3d1qqqq$!!!!!!    o       #")
renderLine(17,"h                  ### SS        SS ###qq3d1qqqqqqrnlqqqq$!!!!!!!!!!        #")
renderLine(18,"h  O     R      >      SS    z   SS    qqrnlqqqqqq9u7qbqqq    !!!!!!!!!     #")
renderLine(19,"h                  ### SS        SS ###qq9u7qqqqqqqqqqqqq$        !!!!!!! P #")
renderLine(20,"h                  #   SSSSSSSSSSSS   #qqqqqqq3d1qq31qqqq$           !!!!   #")
renderLine(21,"h                  #   SSSSSSSSSSSS   #31qqqqqrnlqq97qq3d%            !!!!! #")
renderLine(22,"h                  #                  #97qqqqq9u7qqqqqqrn%             !!!! #")
renderLine(23,"h                  #       #  #       #qqqqqqqqqqqqqqqq9u%               !! #")
renderLine(24,"hHhHhHhHhHhHhHhHhHh#########  #########$$$$$$$$$$$$$$$$$$$############ ######")
renderLine(25,"#HhHhHhHhHhHhHhHhHh#                  #_                 &jjjjjjjjjjjjjjjjjj&")
renderLine(26,"#HHHHHHHHHHHHHHHHHH#                  #  `88@ `88@ `88@  &jjjjjjjjjjjjjjjjjj&")
renderLine(27,"#HHHHHHHHHHHHHHHHHH#                  #  ;::6 ;::6 ;::6  &jjjjjjjjjjjjjjjjjj&")
renderLine(28,"#HHHHHHHHHHHHHHHHHH#                  #  ;::6 ;::6 ;::6  &jjjjjjjjjjjjjjjjjj&")
renderLine(29,"#HHHHHHHHHHHHHHHHHH#vvvvvvvvvvvvvvvvvv#  -..^ -..^ -..^  &jjjjjjjjjjjjjjjjjj&")
renderLine(30,"#HHHHHHHHHHHHHHHHHH#vvvvvvvvvvvvvvvvvv#o       >N        &jjjjjjjjjjjjjjjjjj&")
renderLine(31,"#HHHHHHHHHHHHHHHHHH#vvvvvvvvvvvvvvvvvv#  `88@ `88@ `88@  Cjjjjjjjjjjjjjjjjjj&")
renderLine(32,"#HTHHHHHHHHHHHHHHHH#vvvVVVvvvvvvvvvVVV#  ;::6 ;::6 ;::6  Ajjjjjjjjjjjjjjjjjj&")
renderLine(33,"#HHHHHHHHHHHHHHHHHH#vvvV/VvvvvvvvvvVVV#  ;::6 ;::6 ;::6  &jjjjjjjjjjjjjjjjjj&")
renderLine(34,"#HHHHHHHHHHHHHHHHoH#vvvVVVvvvvvvvvvvvv#  -..^ -..^ -..^  &jjjjjjjjjjjjjjjjJj&")
renderLine(35,"#HHHHHHHHHHHHHHHHHH#vvvvvvvvvvvvvvvvvv#                  &jjjjjjjjjjjjjjjjjj&")
renderLine(36,"###############%%###vvvvvvvvvvvVVvvvVV###################&&&&&&&&&&&&&&&&&%%%")
renderLine(37,"#              rr  #vvvvvvvvvvvVVvvvVV#vvvvvVVvvvvVVvvvvv#      U         4<%")
renderLine(38,"# XXXXXXXXX    rr  #vvvvVVVvvvvvvvvVVV#VVVvvVVvvvvVVvVVvv#      ~ ~~~     52%")
renderLine(39,"# X       X    rr  #VVvvVVVvvvvvvvvVVv#VVVvvvvvvvvvvvVVvv#      ~ ~~~       #")
renderLine(40,"# X XXXXX X    rr  #VVvvvvvvvvvvvvvVVv#vvvvvvvvvVVVvvVVvv#      ~           #")
renderLine(41,"# X >>>>X XXX  rr  #vvvvvvvvvvvVVvvvvv#vvvVVVvvvVVVvvvvvv#      ~           #")
renderLine(42,"# X >>>>X      rr  vvvvvVVVvvvvVVvvvvvvvvvV/Vvvvvvvvvvvvvv   M  ~    ~~~    #")
renderLine(43,"# X >>>>X XXX  rr  #vvvvVVVvvvvvvvvVVv#vvvVVVvvvvvvVVVvvv#      ~    ~~~    #")
renderLine(44,"# X XXXXX X    rr  #VVvvvvvvvvVVVvvVVv#vvvvvvvvVVvvVVVvvv#      ~    ~~~  ~~#")
renderLine(45,"# X       X    rr  #VVvVVVvvvvVtVvvvvv#vVVvvvvvVVvvVVVvVV#      ~         ~~#")
renderLine(46,"# XXXXXXXXX    rr  #vvvVVVvVVvVVVvvVVV#vVVvvvVVvvvvvvvvVV#      ~           #")
renderLine(47,"#              rr  #ovvvvvvVVvvvvvvVVV#vvvvvvVVvvvvvvvvvv#      Y           #")
renderLine(48,"###############%%#################################%%%####### ################")
renderLine(49,"#Z                 #o  #       QQQQ   #mmmmmmmmm# rnl    #            #K   K#")
renderLine(50,"#                  #   #       QQQQ   #mmmmmmmmm##%%%###L#            #  y  #")
renderLine(51,"#  gggggggggggggg  #   #       QQQQ   # mmmmmmmmmmmmmmm# #            #     #")
renderLine(52,"#  g G G~GG~G G g  #  I#       QQQQ   #  mmmmmmmmmmmmmm#L#            #fffff#")
renderLine(53,"#  gG G~GeeG~G Gg  #####       QQQQ   #   mmmmmmmmmmmmm# #        o   #     #")
renderLine(54,"#, gGG|G ~~ G|GGg              QQQQ       mmmmmmmmmmmmm#L#            #fffff#")
renderLine(55,"#  gG G~GeeG~G Gg  #           QQQQ   #   mmmmmmmmmmmmm  #            #     #")
renderLine(56,"#  g G G~GG~G G g  #    O      QQQQ   #  mmmmmmmmmmmmmm  #            #fffff#")
renderLine(57,"#  gggggggggggggg  #           QQQQ   #mmmmmmmmmmmmmmm                #     #")
renderLine(58,"#                  #           QQQQ   #mmmmmmpmmmmmmmm   #            #fffff#")
renderLine(59,"#                  #        k  QQQQ   #mmmmmmmmmmmmmm    #            E     #")
renderLine(60,"###############################ssss####xxxxxxxxxxxxxx########################")

oxyd_shuffle()

xa=random(5)
xb=random(5)
xc=random(5)
xd=random(5)

enigma.KillStone(70+xa,58)
enigma.KillStone(70+xb,56)
enigma.KillStone(70+xc,54)
enigma.KillStone(70+xd,52)

















