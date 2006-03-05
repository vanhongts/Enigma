-- Teamplay Doors, a level for Enigma
-- Copyright (C) 2005 Lukas Schueller
-- Licensed under GPL v2.0 or above
-- Thanks to Petr Machata for his ant.lua script
---------------------------------------------------------------
-- Dec/Jan 05/06: Added abyss, FOLLOW-Scrolling, and
--                changed behaviour of the first doors; Raoul and Andreas

--Environment
Require("levels/lib/ant.lua")

stateA = 0
stateB = 0
stateC = 0
stateD = 0
stateE = 0
function TriggerA()
  if stateA == 0 then
    SendMessage(enigma.GetNamedObject("doorz"), "close");
    SendMessage(enigma.GetNamedObject("doora"), "open");
  end
  stateA = 1 - stateA
end
function TriggerB()
  if stateB == 0 then
    SendMessage(enigma.GetNamedObject("doorz"), "open");
    SendMessage(enigma.GetNamedObject("doora"), "close");
    SendMessage(enigma.GetNamedObject("doorb"), "open");
  end
  stateB = 1 - stateB
end
function TriggerC()
  if stateC == 0 then
    SendMessage(enigma.GetNamedObject("doorb"), "close");
    SendMessage(enigma.GetNamedObject("doorc"), "open");
  end
  stateC = 1 - stateC
  TriggerA()
end
function TriggerD()
  if stateD == 0 then
    SendMessage(enigma.GetNamedObject("doorc"), "close");
    SendMessage(enigma.GetNamedObject("doord"), "open");
  end
  stateD = 1 - stateD
  TriggerB()
end
function TriggerE()
  if stateE == 0 then
    SendMessage(enigma.GetNamedObject("doora"), "open");
    SendMessage(enigma.GetNamedObject("doorb"), "open");
    SendMessage(enigma.GetNamedObject("doorc"), "open");
    SendMessage(enigma.GetNamedObject("doord"), "open");
    SendMessage(enigma.GetNamedObject("doore"), "open");
  end
  stateE = 1 - stateE
  --TriggerC()
end


multiplayer_mode()
cells={}
cells[" "]=cell{floor="fl-himalaya"}
cells["#"]=cell{stone="st-plain"}
cells["x"]=cell{item="it-trigger"}
cells[":"]=cell{floor="fl-abyss"}
map_cell_meaning("A", cell{stone={"st-door_b", {name="doora"}}})
map_cell_meaning("B", cell{stone={"st-door_b", {name="doorb"}}})
map_cell_meaning("C", cell{stone={"st-door_b", {name="doorc"}}})
map_cell_meaning("D", cell{stone={"st-door_b", {name="doord"}}})
map_cell_meaning("E", cell{stone={"st-door_b", {name="doore"}}})
map_cell_meaning("F", cell{stone={"st-door_b", {name="doorf"}}})
map_cell_meaning("G", cell{stone={"st-door_b", {name="doorg"}}})
map_cell_meaning("H", cell{stone={"st-door_b", {name="doorh"}}})
map_cell_meaning("I", cell{stone={"st-door_b", {name="doori"}}})
map_cell_meaning("J", cell{stone={"st-door_b", {name="doorj"}}})
map_cell_meaning("K", cell{stone={"st-door_b", {name="doork"}}})
map_cell_meaning("L", cell{stone={"st-door_b", {name="doorl"}}})
map_cell_meaning("M", cell{stone={"st-door_b", {name="doorm"}}})
map_cell_meaning("N", cell{stone={"st-door_b", {name="doorn"}}})
map_cell_meaning("O", cell{stone={"st-door_b", {name="dooro"}}})
map_cell_meaning("P", cell{stone={"st-door_b", {name="doorp"}}})
map_cell_meaning("Q", cell{stone={"st-door_b", {name="doorq"}}})
map_cell_meaning("R", cell{stone={"st-door_b", {name="doorr"}}})
map_cell_meaning("S", cell{stone={"st-door_b", {name="doors"}}})
map_cell_meaning("Z", cell{stone={"st-door_b", {name="doorz"}}})
map_cell_meaning("a", cell{item={"it-trigger", {action="callback", target="TriggerA"}}})
map_cell_meaning("b", cell{item={"it-trigger", {action="callback", target="TriggerB"}}})
map_cell_meaning("c", cell{item={"it-trigger", {action="callback", target="TriggerC"}}})
map_cell_meaning("d", cell{item={"it-trigger", {action="callback", target="TriggerD"}}})
map_cell_meaning("e", cell{item={"it-trigger", {action="callback", target="TriggerE"}}})
--map_cell_meaning("a", cell{item={"it-trigger", {action="open", target="doora"}}})
--map_cell_meaning("b", cell{item={"it-trigger", {action="open", target="doorb"}}})
--map_cell_meaning("c", cell{item={"it-trigger", {action="open", target="doorc"}}})
--map_cell_meaning("d", cell{item={"it-trigger", {action="open", target="doord"}}})
--map_cell_meaning("e", cell{item={"it-trigger", {action="open", target="doore"}}})
map_cell_meaning("f", cell{item={"it-trigger", {action="openclose", target="doorf"}}})
map_cell_meaning("g", cell{item={"it-trigger", {action="openclose", target="doorg"}}})
map_cell_meaning("h", cell{item={"it-trigger", {action="openclose", target="doorh"}}})
map_cell_meaning("i", cell{item={"it-trigger", {action="openclose", target="doori"}}})
map_cell_meaning("j", cell{item={"it-trigger", {action="openclose", target="doorj"}}})
map_cell_meaning("k", cell{item={"it-trigger", {action="openclose", target="doork"}}})
map_cell_meaning("l", cell{item={"it-trigger", {action="openclose", target="doorl"}}})
map_cell_meaning("m", cell{item={"it-trigger", {action="openclose", target="doorm"}}})
map_cell_meaning("n", cell{item={"it-trigger", {action="openclose", target="doorn"}}})
map_cell_meaning("o", cell{item={"it-trigger", {action="openclose", target="dooro"}}})
map_cell_meaning("p", cell{item={"it-trigger", {action="openclose", target="doorp"}}})
map_cell_meaning("q", cell{item={"it-trigger", {action="openclose", target="doorq"}}})
map_cell_meaning("r", cell{item={"it-trigger", {action="openclose", target="doorr"}}})
map_cell_meaning("s", cell{item={"it-trigger", {action="openclose", target="doors"}}})
level = {
  "############################:::::::::::",
  "#  E    f######    M N #   0:::::::::::",
  "#  ###   ######    ###O#   #:::::::::::",
  "#  De#   gHiJkL    # P #   0:::::::::::",
  "#  ###   ######    #Q###   #:::::::::::",
  "#1 Cd#   ######    # R S   0:::::::::::",
  "#  ####F####################:::::::::::",
  "#2 Bc#   ######   #m n x   0:::::::::::",
  "#  ###   ######   # x o    #:::::::::::",
  "#  Ab#   GhIjKl    x p x   0:::::::::::",
  "#  ###   ######   # q x    #:::::::::::",
  "#  Za#   ######   #x r s   0:::::::::::",
  "############################:::::::::::"
}
oxyd_default_flavor = "a"
set_default_parent(cells[" "])
create_world_by_map(level)
oxyd_shuffle()
SendMessage(enigma.GetNamedObject("doorz"), "open");

display.SetFollowMode(display.FOLLOW_SCROLLING)


















