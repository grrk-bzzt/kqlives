-- grotto2 - "Second, larger part of grotto north of Ekla"

function autoexec()
  refresh()
end


function refresh()
  if (get_progress(P_CANCELROD) > 0) then
    set_mtile(20, 23, 265)
  end

  if (get_treasure(48) == 1) then
    set_mtile(17, 52, 265)
  end

  if (get_treasure(49) == 1) then
    set_mtile(18, 47, 265)
  end
end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 0) then
    combat(24)

  elseif (zn == 1) then
    change_map("grotto", 11, 13, 11, 13)

  elseif (zn == 2) then
    bubble(HERO1, "This door is stuck tight.")

  elseif (zn == 3) then
    if (get_progress(P_CANCELROD) == 0) then
      set_progress(P_CANCELROD, 1)
      sfx(5)
      msg("Rod of Cancellation procured", 255, 0)
      refresh()
    end

  elseif (zn == 4) then
    chest(48, I_MHERB, 1)
    refresh()

  elseif (zn == 5) then
    chest(49, I_HELM1, 1)
    refresh()

  end
end


function entity_handler(en)
  return
end
