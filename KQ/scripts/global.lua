-- Global functions available to all scripts

-- pick one of the args
-- if arg is a table it can have a pr field which gives 
-- its probability of being picked
-- e.g. print(pick(1,2,3))
--      pick({pr=5, name="Rare"}, {pr=95, name="Common"}).name
function pick(...) 
 cumprob=0
 for i=1,arg.n do
  if (istable(arg[i]) ) then
	prob=arg[i].pr or 1
  else
	prob=1
  end
  cumprob=cumprob+prob
 end
 cumprob=krnd(cumprob)
 for i=1,arg.n do
  if (istable(arg[i]) ) then
	prob=arg[i].pr or 1
  else
	prob=1
  end
  cumprob=cumprob-prob
  if (cumprob<0) then
   return arg[i]
  end
 end
end

function Sensar:touch_fire()
	bubble(HERO1, pick ("What th..? Ouch! That's hot!", 
		"There's no way I'm sticking my hand in that fire!", 
		"This feels pretty nice."))
end
function Sarina:touch_fire()
 bubble (HERO1,pick( "Mmm, wood smoke.","Smells like burnt hair. Hey wait... that's MY hair!", "Ooh, cozy."));
end
function Corin:touch_fire()
bubble(HERO1,pick("I sure like fire.", "Watching this is relaxing.","This is making me sleepy."));
end
function Ajathar:touch_fire()
bubble(HERO1,pick("Hmm... I want marshmallows.", "You call this a fire?!","Ah, relaxing."))
end
function Casandra:touch_fire() 
bubble(HERO1, pick( "Something's burning. I hope it's one of those stupid books!","The fire is getting low.","Yessir, this is a fire."));
end
function Temmin:touch_fire()
bubble(HERO1,pick("Ah, the age-old fire.","This needs more coal.","This would be great to read a book next to."))
end
function Ayla:touch_fire()
bubble(HERO1,pick( "I wonder how hot this is?", "Someone should clean all this soot out of here.", "Well, my face is warm now, but my butt is still freezing!"))
end
function Noslom:touch_fire()
bubble(HERO1,pick( "I prefer torches.", "I love the crackle of a good fire.","I wonder if a spell would make this burn brighter?"))
end
function Sensar:read_book()
	bubble(HERO1, pick ("Reading makes me sleepy...","So many books...", "Reading is for wimps."))
end
function Sarina:read_book()
 bubble (HERO1,pick( "Ugh... this would take me forever to read", "I never liked reading", "Who wrote this trash?"))
end
function Corin:read_book()
bubble(HERO1,pick( "Doesn't anybody leave spellbooks lying around?", "Why would I read this?","Can't talk... reading."))
end
function Ajathar:read_book()
bubble(HERO1,pick( "Hmmm... I don't approve of that.","I'm too busy to read now.", "How many books can you write that start with 'The Joy of...'?"))
end
function Casandra:read_book() 
bubble(HERO1, pick( "Boring.", "Somebody should burn these.", "Terrible... just terrible."))
end
function Temmin:read_book()
bubble(HERO1,pick( "If only I had more time...", "So many books...","Some of these are pretty old."))
end
function Ayla:read_book()
bubble(HERO1,pick("I don't have time for this.", "What language is this written in?","The pages are stuck together!?"))
end
function Noslom:read_book()
bubble(HERO1,pick( "Fascinating.","I have this one.","Romance novels... gack!"))
end

-- This function can be called whenever the hero touches a fire
function touch_fire(ent)
party[0]:touch_fire()
end

--  Response for reading a book.
function book_talk(ent)
party[0]:read_book()
end



-- Display bubble text; just concatenate all the args and call the _ex function
-- Args ent  Entity number 
--      ...  Variable number of arguments - text to show
function bubble(ent, ...)
s=""
for i=1,arg.n do
 if (i~=1) then
  s=s.."\n"
 end
 s=s..arg[i]
end
bubble_ex(ent, s)
end

-- See function bubble()
function thought(ent, ...)
s=""
for i=1,arg.n do
 if (i~=1) then
  s=s.."\n"..arg[i]
 else 
  s=s..arg[i]
 end
end
thought_ex(ent, s)
end
