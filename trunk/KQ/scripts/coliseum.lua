-- coliseum - "Coliseum west of Sunarin"

-- /*
-- {
-- Get the 6th PM after the battle
-- Of course, we shouldn't let him/her join except if we do a side-quest.
-- }
-- */


function autoexec()
  set_ent_active(12, 0)
  if (get_progress(P_ROUNDNUM) > 0) then
    place_ent(6, 67, 20+4)
  end
end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 1) then
    set_progress(P_ROUNDNUM, 0)
    set_progress(P_BATTLESTATUS, 0)
    change_map("main", "coliseum")

  elseif (zn == 2) then
    door_in(72, 11+4, 62, 0+4, 82, 14+4)

  elseif (zn == 3) then
    door_in(66, 30+4, 62, 16+4, 70, 33+4)

  elseif (zn == 4) then
    door_in(79, 27+4, 72, 16+4, 86, 30+4)

  elseif (zn == 5) then
    door_out(20, 21)

  elseif (zn == 6) then
    door_out(30, 22)

  elseif (zn == 7) then
    door_out(40, 20)

  elseif (zn == 8) then
    inn("The Coliseum Inn", 250, 1)

  elseif (zn == 9) then
    shop(22)

  elseif (zn == 10) then
    door_out(get_marker_tilex("battle"), get_marker_tiley("battle"))
  
  elseif (zn == 11) then
    set_save(0)

  elseif (zn == 12) then
    bubble(HERO1, "Hmm. The sun feels very warm right here.")
    set_save(1)

  elseif (zn == 13) then
    touch_fire(party[0])

  elseif (zn == 14) then
    door_in(66, 23, 62, 16+4, 70, 33+4)
    
  elseif (zn == 15) then
    if (get_progress(P_BATTLESTATUS) > 0) then
      bubble(HERO1, "I'm not ready yet.")
      return
    end
    set_run(0)
    set_progress(P_USEITEMINCOMBAT, 1)
    combat(11 + get_progress(P_ROUNDNUM))
    set_progress(P_USEITEMINCOMBAT, 0)
    set_run(1)
    warp(get_marker_tilex("battle"), get_marker_tiley("battle") - 3, 16)
    if (get_alldead() == 1) then
      set_progress(P_BATTLESTATUS, 2)
      set_party_hp(party[0], 1)
      set_alldead(0)
    else
      set_progress(P_BATTLESTATUS, 1)
    end

  end
end


function entity_handler(en)
  if (en == 0) then
    bubble(en, "I actually made it to the second round!")

  elseif (en == 1) then
    bubble(en, "I'm out of practice.")

  elseif (en == 2) then
    bubble(en, "I can't stand waiting like this!")

  elseif (en == 3) then
    bubble(en, "There's not enough women in these competitions.")

  elseif (en == 4) then
    bubble(en, "I came all this way and then found out that I didn't have enough money for the registration fee!")

  elseif (en == 5) then
    bubble(en, "I'm waiting for my next match.")

  elseif (en == 6) then
    local a, b

    if (get_progress(P_OPALSHIELD) == 1) then
      bubble(en, "This is where you register.")
      bubble(en, "Hey, wait a minute... you're the one who defeated Trayor. We can't let you register again... you're too good for these guys.")
      return
    end
    if (get_progress(P_ROUNDNUM) == 0) then
      bubble(en, "This is where you register.")
      if (get_progress(P_FIRSTTIME) == 0) then
        bubble(en, "Before I explain the rules, I should tell you that the registration fee is 2000 gp!")
        bubble(en, "If you lose even once, you are done... and will have to register again.")
        bubble(en, "As well, after you register you can't leave the coliseum grounds. If you leave, you will be forced to register again.")
        bubble(en, "Now, let's talk about the actual fights!")
        bubble(en, "The first three battles are more or less just preliminary fights to make sure you have what it takes.")
        bubble(en, "If you win the first three rounds, you qualify for more advanced matches.")
        bubble(en, "The more advanced matches are against more skilled opponents and will determine whether or not your skills are balanced enough.")
        bubble(en, "If you manage to defeat the advanced warriors, you will be given a chance to fight Trayor for the grand prize.")
        a = prompt(en, 2, 0, "So, are you willing to pay the",
                             "2000 gp entrance fee?",
                             "  no  ",
                             "  yes ")
        set_progress(P_FIRSTTIME, 1)
      else
        a = prompt(en, 2, 0, "The registration fee is 2000 gp.",
                             "Are you in?",
                             "  no  ",
                             "  yes ")
      end
      if (a == 0) then
        bubble(en, "Oh well. Come back when you're ready.")
      else
        if (get_gp() >= 2000) then

          set_gp(get_gp() - 2000)
          bubble(en, "Consider yourself registered. After each battle, come back and talk to me.")
          set_ent_script(6, "U2R1F2")
          wait_for_entity(6, 6)
          set_progress(P_ROUNDNUM, 1)
          set_progress(P_BATTLESTATUS, 0)
        else
          bubble(en, "Umm... you don't seem to have enough. I would suggest selling some junk. In any case, you can't register at this point.")
        end
      end
    else
      a = get_progress(P_BATTLESTATUS)
      if (a == 0) then
        if (get_progress(P_ROUNDNUM) == 7) then
          bubble(en, "Wow! This is it... your next battle is with Trayor. Good luck... you'll need it!")
        else
          bubble(en, "Battle number "..get_progress(P_ROUNDNUM)..". Just head on through that door when you are ready.")
        end
      elseif (a == 1) then
        if (get_progress(P_ROUNDNUM) == 7) then
          bubble(en, "Astounding! Whoever you are, you're going to go far!")
          bubble(en, "Well, I guess this is yours.")
          sfx(5)
          msg("Opal Shield procured", 255, 0, xofs, yofs)
          set_progress(P_OPALSHIELD, 1)
          if (get_ent_tilex(HERO1) == get_ent_tilex(6)) then
            set_ent_script(HERO1, "L1")
            wait_for_entity(HERO1, HERO1)
          end

          if (get_numchrs() == 1) then
            move_entity(HERO1, "prebattle", 0)
            wait_for_entity(HERO1, HERO1)
            set_ent_facing(HERO1, FACE_UP)
          else
            move_entity(HERO1, "prebattle", 0)
            move_entity(HERO2, get_marker_tilex("prebattle"), get_marker_tiley("prebattle") + 1, 0)
            wait_for_entity(HERO1, HERO2)
            set_ent_facing(HERO1, FACE_UP)
            set_ent_facing(HERO2, FACE_UP)
            orient_heroes()
          end
          set_ent_script(6, "L1D2")
          wait_for_entity(6, 6)
          bubble(6, "Good luck in your endeavours.")
	  if (get_numchrs() > 1) then -- Skip this partnering up bit.
	     return
	  end
          bubble(en, "Good luck in your endeavours.")
          if (get_progress(P_OLDPARTNER) > 0) then
            set_progress(P_FINALPARTNER, get_progress(P_OLDPARTNER))
          else
            if (party[0] == Casandra) then
              if (krnd(10) < 5) then
                set_progress(P_FINALPARTNER, TEMMIN + 1)
              end
            end
            if (party[0] == Temmin) then
              if (krnd(10) < 5) then
                set_progress(P_FINALPARTNER, CASANDRA + 1)
              end
            end
            while (get_progress(P_FINALPARTNER) == 0) do
              z = krnd(8)
              if (z ~= party[0].id) then
                set_progress(P_FINALPARTNER, z + 1)
              end
            end
          end
          set_ent_id(en, get_progress(P_FINALPARTNER) - 1)
          set_ent_active(en, 1)
        else
          bubble(en, "Congratulations! You should have a rest or heal up. Come back and talk to me again after you've prepared.")
          set_progress(P_BATTLESTATUS, 0)
          set_progress(P_ROUNDNUM, get_progress(P_ROUNDNUM) + 1)
        end
      elseif (a == 2) then
        bubble(en, "Ooh... tough luck!")
        set_progress(P_BATTLESTATUS, 0)
        set_progress(P_ROUNDNUM, 0)
        set_ent_script(HERO1, "D3F1")
        wait_for_entity(0, 0)
        set_ent_script(6, "L1D2")
        wait_for_entity(6, 6)
        play_map_song()
      end
    end

  elseif (en == 7) then
    bubble(en, "Losing isn't so bad.")
    bubble(en, "No wait, let me rephrase that!")
    bubble(en, "Losing sucks!")

  elseif (en == 8) then
    bubble(en, "What a waste of money.")
    bubble(HERO1, "How many matches did you fight?")
    bubble(en, "Matches? I haven't left the bar yet!")

  elseif (en == 9) then
    bubble(en, "These fights are quite interesting.")

  elseif (en == 10) then
    bubble(en, "Wine puts losing in a whole new perspective.")

  elseif (en == 11) then
    bubble(en, "The Coliseum hasn't been the same since they discontinued betting on the fights.")

  elseif (en == 12) then
    if (get_progress(P_OLDPARTNER) == 0) then
      bubble(en, "Wow! I saw you fighting. You cleaned up!")
      bubble(en, "Oh yeah! I'm here because I ran into some guy named Derig. Well, he actually came looking for me.")
      bubble(en, "Anyways, he wanted me to come and find you.")
      bubble(HERO1, "Derig! What does he want now?")
      bubble(en, "He said that he found out where all of the Opal armor is and he sent me to tell you.")
      bubble(en, "He also figured that if you didn't have a partner by now, that I should make my services available to you.")
      bubble(en, "And let me just say that after travelling all this way to find you... I won't take no for an answer!")
      bubble(HERO1, "Well, in that case, welcome to the team.")
      bubble(HERO1, "Now, I already found the Opal Helmet and the Opal Shield.")
      bubble(en, "So what about the other stuff? Do you already know where the Band and Armor are?")
      bubble(HERO1, "No... no, I don't know where the other pieces are.")
      bubble(en, "Alrighty then, down to business. The Band belongs to a merchant in Ajantara south of here.")
      bubble(en, "The Armor, however, is hidden deep in a cavern east of here.")
      bubble(HERO1, "Well, where should we go first?")
      bubble(en, "Derig said to get the Armor last and since all that leaves is the Band, then we should go for that.")
      bubble(HERO1, "Fair enough... let's roll.")
      add_chr(get_progress(P_FINALPARTNER) - 1)
      LOC_partner_up()
      set_progress(P_FINALPARTNER, 0)
      copy_ent(en, HERO2)
      set_ent_active(en, 0)
      orient_heroes()
      drawmap()
      screen_dump()
      msg("$1 joined!", 255, 0)
    else
      bubble(en, "Hey... long time no see. I saw you fighting. You were amazing!")
      bubble(en, "Oh yeah! I'm here because I ran into some guy named Derig. Well, he actually came looking for me.")
      bubble(en, "He knew that we were separated and he gave me some information to bring back.")
      bubble(HERO1, "Speaking of that... where did you go?")
      bubble(en, "Heh... when we hit the teleporter, it sent me back to Ekla.")
      bubble(en, "I was on the way back to the teleporter when I ran into Derig... and now I'm here.")
      bubble(HERO1, "So, what did Derig tell you?")
      bubble(en, "He told me where all the Opal stuff is... the Shield, the Band, the Armor, and the Helmet.")
      bubble(HERO1, "I already snagged the Helmet and the Shield... where are the other parts?")
      bubble(en, "You already found the Helmet and Shield! Wow! Well, the Band is in a town called Ajantara to the south.")
      bubble(en, "The Armor is deep in a cavern to the east. And Derig said to get the Armor last, so we should go to Ajantara first. Okay?")
      bubble(HERO1, "No problem... let's go.")
      add_chr(get_progress(P_FINALPARTNER) - 1)
      LOC_partner_up()
      set_progress(P_FINALPARTNER, 0)
      copy_ent(en, HERO2)
      set_ent_active(en, 0)
      orient_heroes()
      drawmap()
      screen_dump()
      msg("$1 joined!", 255, 0)
    end

  end
end


function LOC_partner_up()
  local a

  a = get_party_xp(party[0]) - get_party_xp(get_pidx(1))
  if (a > 5000) then
    a = a - 5000
  end
  a = a + krnd(500)
  give_xp(get_pidx(1), a, 1)
  a = get_progress(P_FINALPARTNER) - 1
  if (a == SENSAR) then
    set_all_equip(0, I_HAMMER2, I_SHIELD3, I_CAP3, I_ARMOR5, I_BAND2, 0)
  elseif (a == SARINA) then
    set_all_equip(1, I_SPEAR2, 0, I_CAP3, I_SUIT3, I_BAND2, 0)
  elseif (a == CORIN) then
    set_all_equip(2, I_ROD3, I_SHIELD3, I_CAP2, I_ROBE3, I_BAND2, 0)
  elseif (a == AJATHAR) then
    set_all_equip(3, I_HAMMER2, I_SHIELD3, I_CAP2, I_ROBE3, I_BAND2, 0)
  elseif (a == CASANDRA) then
    set_all_equip(4, I_STAFF2, 0, I_CAP2, I_ROBE3, I_BAND2, 0)
  elseif (a == TEMMIN) then
    set_all_equip(5, I_SWORD4, I_SHIELD3, I_HELM2, I_ARMOR5, I_BAND2, 0)
  elseif (a == AYLA) then
    set_all_equip(6, I_SWORD4, I_SHIELD3, I_CAP3, I_SUIT3, I_BAND2, 0)
  elseif (a == NOSLOM) then
    set_all_equip(7, I_ROD3, I_SHIELD3, I_CAP2, I_ROBE3, I_BAND2, 0)
  end
end
