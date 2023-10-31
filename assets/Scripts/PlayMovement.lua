
function update()
	if KeyIsDown(KEYBOARD.D) then
		SetPosition(0, 10, 0, 0)
	elseif KeyIsDown(KEYBOARD.A) then
		SetPosition(0, -10, 0, 0)
	elseif KeyIsDown(KEYBOARD.W) then
		SetPosition(0, 0, 10, 0)
	elseif KeyIsDown(KEYBOARD.S) then
		SetPosition(0, 0, -10, 0)
	end
	--Future: Update the collide to take tag
end

update()
