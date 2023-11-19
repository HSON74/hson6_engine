
function update()
	if KeyIsDown(KEYBOARD.D) then
		EAddVelocity(10, 0, 0)
	elseif KeyIsDown(KEYBOARD.A) then
		EAddVelocity(-10, 0, 0)
	elseif KeyIsDown(KEYBOARD.W) then
		--AddVelocity(0, 0, 10, 0)
		EAddForce(5)
		--EToggle(3)
	elseif KeyIsDown(KEYBOARD.S) then
		EAddVelocity(0, -10, 0)
	end
	--Future: Update the collide to take tag
	
end

update()
