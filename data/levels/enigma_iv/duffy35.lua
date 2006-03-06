-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Labyrinth of Puzzles

levelw=77
levelh=61

create_world( levelw, levelh)
enigma.SetCompatibility("oxyd1")
enigma.TwoPlayerGame = TRUE

fill_floor("fl-hay", 0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c =="#" then
            set_stone( "st-greenbrown", i-1, line)
        elseif c == "o" then
            oxyd( i-1, line)
        elseif c == "*" then
            set_stone( "st-brownie", i-1, line)
        elseif c == "&" then
            set_stone( "st-brownie", i-1, line)
            hammer(i-1,line)
        elseif c == "!" then
            abyss(i-1,line)
            --			fill_floor("fl-water", i-1,line, 1,1)
        elseif c=="z" then
            set_actor("ac-blackball", i-.5,line+.5)
--            set_item("it-yinyang", i-1, line+.5)
        elseif c=="y" then
            set_actor("ac-whiteball", i-.5,line+.5)
--            set_item("it-yinyang", i-1, line+.5)
        elseif c == "g" then
            draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
        elseif c=="+" then
            set_stone( "st-wood", i-1, line)
        elseif c=="W" then
            fill_floor("fl-water", i-1,line, 1,1)
        elseif c=="d" then
            set_stone("st-oneway-s", i-1,line)	
        elseif c=="l" then
            set_stone("st-oneway-w", i-1,line)
        elseif c=="R" then
            set_stone("st-oneway-e", i-1,line)
        elseif c=="u" then
            set_stone("st-oneway-n", i-1,line)
        elseif c=="L" then
            set_attrib(laser(i-1,line, TRUE, NORTH), "name", "laser")
        elseif c=="M" then
            set_attrib(laser(i-1,line, FALSE, SOUTH), "name", "laser2")
        elseif c=="K" then
            set_stone("st-key_a", i-1,line, {action="openclose", target="door5"})
        elseif c=="k" then
            set_item("it-key_a", i-1,line)
        elseif c=="V" then
            set_stone("st-key_b", i-1,line, {action="openclose", target="door3"})
        elseif c=="v" then
            set_item("it-key_b", i-1,line)
        elseif c=="`" then
            mirrorp(i-1,line,FALSE,TRUE, 2)
        elseif c=="/" then
            mirrorp(i-1,line,FALSE,TRUE, 4)
        elseif c=="f" then	
            set_item("it-floppy", i-1,line)
        elseif c=="h" then
            hammer(i-1,line)
        elseif c=="H" then
            set_stone("st-stone_break", i-1,line)
        elseif c=="i" then
            set_stone("st-invisible", i-1,line)
        elseif c=="Q" then
            set_stone("st-bombs", i-1,line)
        elseif c=="F" then
            set_stone( "st-floppy", i-1,line,{action="onoff",target="laser2"})
        elseif c=="A" then
            doorh( i-1,line, {name="door1"})
        elseif c=="B" then
            doorv( i-1,line, {name="door2"})
        elseif c=="C" then
            doorh( i-1,line, {name="door3"})
        elseif c=="4" then
            doorv( i-1,line, {name="door6"})
        elseif c=="J" then
            doorv( i-1,line, {name="door7"})
        elseif c=="<" then
            doorv( i-1,line, {name="door8"})
        elseif c == "D" then
            set_stone("st-bolder-s", i-1,line)
        elseif c == "b" then
            yy1( "black",  i-1, line)
        elseif c == "w" then
            yy1( "white",  i-1, line)
        elseif c == "X" then
            draw_stones("st-yinyang1", { i-1,line}, {0,1},1)
        elseif c=="P" then
            set_item("it-brush",i-1,line)
        elseif c=="r" then
            set_stone("st-stone_break", i-1,line)
            fill_floor("fl-ice_001", i-1,line, 1,1)
        elseif c=="p" then
            draw_stones("st-stonebrush", {i-1,line}, {1,0},1)
        elseif c=="s" then
            set_stone("st-stoneimpulse",i-1,line)
        elseif c=="S" then
            set_stone( "st-stoneimpulse-hollow", i-1,line)
        elseif c == "a" then
            set_item("it-trigger", i-1,line, {action="openclose", target="door1"})
        elseif c == "8" then
            set_item("it-key_c", i-1,line)
        elseif c == "3" then
            set_item("it-coin2", i-1,line)
        elseif c=="9" then
            set_stone("st-switch", i-1,line, {action="openclose",target="door2"})
        elseif c=="j" then
            set_stone("st-switch", i-1,line, {action="openclose",target="door7"})
        elseif c=="," then
            set_stone("st-switch", i-1,line, {action="openclose",target="door8"})
        elseif c=="6" then
            set_stone("st-key_c", i-1,line, {action="openclose", target="door4"})
        elseif c=="N" then
            set_stone("st-coinslot", i-1,line, {action="callback", target="funcc"})
        elseif c == "5" then
            set_stone("st-door_b", i-1,line, {name="door4", type="v"})
        elseif c == "2" then
            set_stone("st-door_b", i-1,line, {name="door5", type="v"})
        elseif c=="=" then
            fill_floor("fl-space", i-1,line, 1,1)
        elseif c=="m" then
            fill_floor("fl-metal",i-1,line, 1,1)
        elseif c=="c" then
            fill_floor("fl-leaves", i-1,line,1,1)
        elseif c=="n" then
            fill_floor("fl-sand", i-1,line,1,1)
        elseif c == "$" then
            mirrorp(i-1,line,TRUE,FALSE, 2)
        elseif c == "%" then
            set_stone( "st-glass", i-1, line)
            fill_floor("fl-water", i-1,line, 1,1)
        elseif c == "^" then
            set_stone( "st-glass", i-1, line)
        elseif c=="G" then
            set_stone( "st-greenbrown", i-1, line)
            fill_floor("fl-space", i-1,line, 1,1)
        end
    end	
end

function yy1( color, x, y)
    stone = format( "st-%s4", color)
    set_stone( stone, x, y)
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00,"ssssssssssssss###############################################################")
renderLine(01,"s z                                #                   #                  y #")
renderLine(02,"s ####### ######################## # #u#####b######### #A############ ##### #")
renderLine(03,"s # # # # #nnnnnnn#              # # #     # #   v     #         #        # #")
renderLine(04,"# # # # # #nWWWWWn# ############ # # # ### # #   #########       ######## # #")
renderLine(05,"# # # # # #nWWWWWn# #          # #   w #   # #   *SSSSSSS#    # ###SSSSS# # #")
renderLine(06,"# # # # # #nWWaWWn# # ######## # ##### # ### #   #######S#    #sSSSS # S# # #")
renderLine(07,"# # # # # #nWWWWWn# # #        # WWWW### # # #         #S######      & S# # #")
renderLine(08,"# # #   # #nWWWWWn# # # ##########   #   * # #         #SSSSSSSSSSSSSSSS# # #")
renderLine(09,"# # # # # #nWWWWWn# # #      #   #   ####u## #     ###################### # #")
renderLine(10,"# # # # # #nWWWWWn# # ## ###   # #  +  sskl  #  L  #          B           # #")
renderLine(11,"# # #K# # #nWWWWWn# # #  #Q##### #  +  #sss### ##### ########## ########### #")
renderLine(12,"# # ### # #nWWWWWn# # # ##     # #  ## #######       #          #N          #")
renderLine(13,"# #     # #nn++gnn# # #  #  $    #   +   #   ######### ####################=#")
renderLine(14,"# #####    c++c++c# # ## # ###########    i# i         #==================G=#")
renderLine(15,"#          ccccccc#   #  #ccccccccM#      ## ## ########==================G=#")
renderLine(16,"# #####################CF#cc++++ccc#i# #  i#  # #=========================G=#")
renderLine(17,"# b         g +g         ^cc+cc$c#c# # # # ## # #===mmm======G============G=#")
renderLine(18,"# ########################ccgcc+ccc# # #i#    # #===mmm=====G=====mm===mm=G=#")
renderLine(19,"# #                      #cc++++ccc# # # ## #   #===mmm====G======mm===mm=G=#")
renderLine(20,"# # #################### #nnnnnnnnn#   #    #####==========G==============G=#")
renderLine(21,"# #         #V           #WWWWWWWWW### # ####==============GGGGGG=========G=#")
renderLine(22,"# ######### ##############W#WWWWW%WWW#   #===========mmm===G========mm====G=#")
renderLine(23,"# R   #   #    #95       #$WWW%WWWWWW#####===========mmm===G========mm====G=#")
renderLine(24,"# ### # # #### ####W+sss ##WWW%WWWWWf#    =================G6=============G=#")
renderLine(25,"# #   # #    #  # # ###ss############# ####################################=#")
renderLine(26,"# # ### ####### # #   #                234  X X X #    p   p      p       #=#")
renderLine(27,"# #       #   # # ### # ##################  X X X # pp   p   p pppp ppppp #=#")
renderLine(28,"# #######   # # #   # # #oooooooooooooooo#XXX X X #  pp  pp p     p     p #=#")
renderLine(29,"# l   # # ### # # # # # #rrrrrrrrrrrrrrrr#    X X #p  pppp p  ppp ppp p p #=#")
renderLine(30,"# ### # # #   # # #   # #rrrrrrrrrrrrrrrr#XXXXX X #        pppp p   p p p #=#")
renderLine(31,"# # # #   # ### # ### # #rrrrrrrrrrrrrrrr#      X # p pp pp   p   p ppp p #=#")
renderLine(32,"# # # #####     # # ### #rrrrrrrrrrrrrrrr#XXXXXXX ### #  pp p ppppp     p #=#")
renderLine(33,"# #         ##### # # # #rrrrrrrrrrrrrrrr#          P #p    p  p    ppp p #=#")
renderLine(34,"# ###########           #rrrrrrrrrrrrrrrr##############pppppppppppppp   p #=#")
renderLine(35,"#             ###########rrrrrrrrrrrrrrrr< J   X         p        p p ppp #=#")
renderLine(36,"###############         ##################b#w# X ######p pppppppp p   p   #=#")
renderLine(37,"                                         #,#j#####     p        p ppppp pp#=#")
renderLine(38,"                                         #####         pppppppp p         #=#")
renderLine(39,"                                                              p ppppppppp #=#")
renderLine(40,"                                                              p           #=#")
renderLine(41,"                                                              pppppppppppp#=#")
renderLine(42,"                                                                          #=#")
renderLine(43,"                                                                          #=#")
renderLine(44,"                                                                          #=#")
renderLine(45,"                                                                          #=#")
renderLine(46,"                                                                          #=#")
renderLine(47,"                                                                          #=#")
renderLine(48,"                                                                          #=#")
renderLine(49,"                                                                          #=#")
renderLine(50,"                                                                          #=#")
renderLine(51,"                                                                          #=#")
renderLine(52,"                                                                          #=#")
renderLine(53,"                                                                          #=#")
renderLine(54,"                                                                          #=#")
renderLine(55,"                                                                          #=#")
renderLine(56,"                                                                          #=#")
renderLine(57,"                                                                          #=#")
renderLine(58,"                                                                         ##=#")
renderLine(59,"                                                                         #8=#")
renderLine(60,"                                                                         ####")

oxyd_shuffle()

set_stone( "st-greenbrown", 27, 22)
set_stone( "st-greenbrown", 29, 24)

fill_floor("fl-leaves", 31,17,1,1)
fill_floor("fl-leaves", 33,17,1,1)
fill_floor("fl-leaves", 34,15,1,1)

fill_floor("fl-sand", 13,13,1,1)
fill_floor("fl-sand", 14,13,1,1)
fill_floor("fl-sand", 15,13,1,1)

fill_floor("fl-water", 27,22, 1,1)
fill_floor("fl-water", 26,23, 1,1)
fill_floor("fl-water", 26,24, 1,1)

fill_floor("fl-leaves", 12,14,1,1)
fill_floor("fl-leaves", 13,14,1,1)
fill_floor("fl-leaves", 15,14,1,1)
fill_floor("fl-leaves", 16,14,1,1)

fill_floor("fl-leaves", 28,16,1,1)
fill_floor("fl-leaves", 29,16,1,1)
fill_floor("fl-leaves", 30,16,1,1)
fill_floor("fl-leaves", 31,16,1,1)
fill_floor("fl-leaves", 28,19,1,1)
fill_floor("fl-leaves", 29,19,1,1)
fill_floor("fl-leaves", 30,19,1,1)
fill_floor("fl-leaves", 31,19,1,1)
fill_floor("fl-leaves", 28,17,1,1)
fill_floor("fl-leaves", 28,18,1,1)
fill_floor("fl-leaves", 31,17,1,1)
fill_floor("fl-leaves", 31,18,1,1)
fill_floor("fl-space", 74,59, 1,1)
fill_floor("fl-space", 60,24, 1,1)

fill_floor("fl-sand", 36,24,1,1)

fill_floor("fl-ice_001", 25,28, 1,1)
fill_floor("fl-ice_001", 26,28, 1,1)
fill_floor("fl-ice_001", 27,28, 1,1)
fill_floor("fl-ice_001", 28,28, 1,1)
fill_floor("fl-ice_001", 29,28, 1,1)
fill_floor("fl-ice_001", 30,28, 1,1)
fill_floor("fl-ice_001", 31,28, 1,1)
fill_floor("fl-ice_001", 32,28, 1,1)
fill_floor("fl-ice_001", 33,28, 1,1)
fill_floor("fl-ice_001", 34,28, 1,1)
fill_floor("fl-ice_001", 35,28, 1,1)
fill_floor("fl-ice_001", 36,28, 1,1)
fill_floor("fl-ice_001", 37,28, 1,1)
fill_floor("fl-ice_001", 38,28, 1,1)
fill_floor("fl-ice_001", 39,28, 1,1)
fill_floor("fl-ice_001", 40,28, 1,1)

function funcc()
    SendMessage("door6", "open")
end

display.SetFollowMode(display.FOLLOW_SCROLLING)
















