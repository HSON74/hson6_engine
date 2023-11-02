
function update()
	if KeyIsDown(KEYBOARD.D) then
		AddVelocity(0, 10, 0, 0)
	elseif KeyIsDown(KEYBOARD.A) then
		AddVelocity(0, -10, 0, 0)
	elseif KeyIsDown(KEYBOARD.W) then
		--AddVelocity(0, 0, 10, 0)
		AddForce(0,10)
	elseif KeyIsDown(KEYBOARD.S) then
		AddVelocity(0, 0, -10, 0)
	end
	--Future: Update the collide to take tag
end

update()
