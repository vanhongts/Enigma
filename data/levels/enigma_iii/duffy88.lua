-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Bomb Shelter

levelw=21
levelh=150

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE

enigma.SlopeForce=90

fill_floor("fl-abyss", 0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c =="#" then
            set_stone( "st-greenbrown", i-1, line)
            set_floor("fl-hay",i-1,line)
        elseif c =="%" then
            set_stone( "st-invisible", i-1, line)
        elseif c == "o" then
            oxyd( i-1, line)
            set_floor("fl-hay",i-1,line)
        elseif c == "*" then
            set_stone( "st-brownie", i-1, line)
            set_floor("fl-hay",i-1,line)
        elseif c == "!" then
            abyss(i-1,line)
        elseif c == "~" then
            set_floor("fl-water",i-1,line)
        elseif c=="z" then
            set_actor("ac-blackball", i-.5,line+.5, {player=0})
            set_floor("fl-hay",i-1,line)
        elseif c == "g" then
            draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
            set_floor("fl-hay",i-1,line)
        elseif c=="+" then
            set_stone( "st-wood", i-1, line)
            set_floor("fl-hay",i-1,line)
        elseif c=="=" then
            set_floor("fl-space",i-1,line)
        elseif c=="x" then
            set_floor("fl-hay",i-1,line)
        elseif c =="S" then
            set_stone( "st-swap", i-1, line)
            set_floor("fl-hay",i-1,line)
        elseif c =="P" then
            set_stone( "st-pull", i-1, line)
            set_floor("fl-hay",i-1,line)
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
        elseif c=="R" then
            set_actor("ac-rotor", i-.5,line+.5, {range=1000, force=93})
            set_floor("fl-gradient",  i-1,  line, {type=2})	
        elseif c=="B" then
            yy1( "black",  i-1, line)
            set_floor("fl-gradient",  i-1,  line, {type=2})
        elseif c=="U" then
            set_stone("st-oneway-n", i-1,line)
            set_floor("fl-hay",i-1,line)
        end
    end	
end

function yy1( color, x, y)
    stone = format( "st-%s4", color)
    set_stone( stone, x, y)
end

renderLine(00,"%%%%%%%%%%%%%%%%%%%%")
renderLine(01,"%   o    ###    o  %")
renderLine(02,"%   xxxxx*x*xxxxx  %")
renderLine(03,"%   x    #U#    x  %")
renderLine(04,"%   xxxx !u! xxxx  %")
renderLine(05,"%   x  o !u! o  x  %")
renderLine(06,"%   x  o !u! o  x  %")
renderLine(07,"%  oxxxx !u! xxxxo %")
renderLine(08,"%   o  x !u! x  o  %")
renderLine(09,"%   o  x !u! x  o  %")
renderLine(10,"%   x  x !u! x  x  %")
renderLine(11,"%   xxxx !u! xxxx  %")
renderLine(12,"%        #u#       %")
renderLine(13,"%        #u#       %")
renderLine(14,"%        #R#       %")
renderLine(15,"%        #R#       %")
renderLine(16,"%        #R#       %")
renderLine(17,"%        #u#       %")
renderLine(18,"%        #u#       %")
renderLine(19,"%        #u#       %")
renderLine(20,"%        #u#       %")
renderLine(21,"%        #u#       %")
renderLine(22,"%        #u#       %")
renderLine(23,"%        #u#       %")
renderLine(24,"%        #u#       %")
renderLine(25,"%        #u#       %")
renderLine(26,"%        #u#       %")
renderLine(27,"%        #u#       %")
renderLine(28,"%        #u#       %")
renderLine(29,"%        #u#       %")
renderLine(30,"%        #u#       %")
renderLine(31,"%        #u#       %")
renderLine(32,"%        #u#       %")
renderLine(33,"%        #u#       %")
renderLine(34,"%        #u#       %")
renderLine(35,"%        #u#       %")
renderLine(36,"%        #u#       %")
renderLine(37,"%        #u#       %")
renderLine(38,"%        #u#       %")
renderLine(39,"%        #u#       %")
renderLine(40,"%        #u#       %")
renderLine(41,"%        #u#       %")
renderLine(42,"%        #u#       %")
renderLine(43,"%        #u#       %")
renderLine(44,"%        #u#       %")
renderLine(45,"%        #u#       %")
renderLine(46,"%        #u#       %")
renderLine(47,"%        #u#       %")
renderLine(48,"%        #u#       %")
renderLine(49,"%        #u#       %")
renderLine(50,"%        #u#       %")
renderLine(51,"%        #u#       %")
renderLine(52,"%        #u#       %")
renderLine(53,"%        #u#       %")
renderLine(54,"%        #u#       %")
renderLine(55,"%        #u#       %")
renderLine(56,"%        #u#       %")
renderLine(57,"%        #u#       %")
renderLine(58,"%        #u#       %")
renderLine(59,"%        #u#       %")
renderLine(60,"%        #u#       %")
renderLine(61,"%        #u#       %")
renderLine(62,"%        #u#       %")
renderLine(63,"%        #u#       %")
renderLine(64,"%        #u#       %")
renderLine(65,"%        #u#       %")
renderLine(66,"%        #u#       %")
renderLine(67,"%        #u#       %")
renderLine(68,"%        #u#       %")
renderLine(69,"%        #u#       %")
renderLine(70,"%        #u#       %")
renderLine(71,"%        #u#       %")
renderLine(72,"%        #u#       %")
renderLine(73,"%        #u#       %")
renderLine(74,"%        #u#       %")
renderLine(75,"%        #u#       %")
renderLine(76,"%        #u#       %")
renderLine(77,"%        #u#       %")
renderLine(78,"%        #u#       %")
renderLine(79,"%        #u#       %")
renderLine(80,"%        #u#       %")
renderLine(81,"%        #u#       %")
renderLine(82,"%        #u#       %")
renderLine(83,"%        #u#       %")
renderLine(84,"%        #u#       %")
renderLine(85,"%        #u#       %")
renderLine(86,"%        #u#       %")
renderLine(87,"%        #u#       %")
renderLine(88,"%        #u#       %")
renderLine(89,"%        #u#       %")
renderLine(90,"%        #u#       %")
renderLine(91,"%        #u#       %")
renderLine(92,"%        #u#       %")
renderLine(93,"%        #u#       %")
renderLine(94,"%        #u#       %")
renderLine(95,"%        #u#       %")
renderLine(96,"%        #u#       %")
renderLine(97,"%        #u#       %")
renderLine(98,"%        #u#       %")
renderLine(99,"%        #u#       %")
renderLine(100,"%        #u#       %")
renderLine(101,"%        #u#       %")
renderLine(102,"%        #u#       %")
renderLine(103,"%        #u#       %")
renderLine(104,"%        #u#       %")
renderLine(105,"%        #u#       %")
renderLine(106,"%        #u#       %")
renderLine(107,"%        #u#       %")
renderLine(108,"%        #u#       %")
renderLine(109,"%        #u#       %")
renderLine(110,"%        #u#       %")
renderLine(111,"%        #u#       %")
renderLine(112,"%        #u#       %")
renderLine(113,"%        #u#       %")
renderLine(114,"%        #u#       %")
renderLine(115,"%        #u#       %")
renderLine(116,"%        #u#       %")
renderLine(117,"%        #u#       %")
renderLine(118,"%        #u#       %")
renderLine(119,"%        #u#       %")
renderLine(120,"%        #u#       %")
renderLine(121,"%        #u#       %")
renderLine(122,"%        #u#       %")
renderLine(123,"%        #u#       %")
renderLine(124,"%        #u#       %")
renderLine(125,"%        #u#       %")
renderLine(126,"%        #u#       %")
renderLine(127,"%        #u#       %")
renderLine(128,"%        #u#       %")
renderLine(129,"%        #u#       %")
renderLine(130,"%        #u#       %")
renderLine(131,"%        #u#       %")
renderLine(132,"%        #u#       %")
renderLine(133,"% ########u####### %")
renderLine(134,"% #xxxxxxxxxxxxxx# %")
renderLine(135,"% #xxxxxxxxxxxxxx# %")
renderLine(136,"% #xxxxxSgggSxxxx# %")
renderLine(137,"% #xxx*xgggggx*xx# %")
renderLine(138,"% #xxx*xggzggx*xx# %")
renderLine(139,"% #xxx*xgggggx*xx# %")
renderLine(140,"% #xxxxxSgggSxxxx# %")
renderLine(141,"% #xxxxxxxxxxxxxx# %")
renderLine(142,"% #xxxxxxxxxxxxxx# %")
renderLine(143,"% ################ %")
renderLine(144,"%%%%%%%%%%%%%%%%%%%%")

oxyd_shuffle()















