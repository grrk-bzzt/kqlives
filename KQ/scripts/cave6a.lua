-- cave6a - "Cave north of Ajantara"

-- /*
-- {
-- It would be really good if we could get some sort of cave town 2/ people
-- and the 7th PM.
-- }
-- */


function autoexec()
  return
end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("main", 264, 165, 264, 165)

  elseif (zn == 2) then
    change_map("cave6b", 0, 0, 0, 0)

  end
end


function entity_handler(en)
  return
end