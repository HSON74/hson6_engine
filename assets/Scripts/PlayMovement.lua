
function update()
	if KeyIsDown(KEYBOARD.D) then
		EAddVelocity(20, 0, 0)
	end
	if KeyIsDown(KEYBOARD.A) then
		EAddVelocity(-20, 0, 0)
	end
	if KeyIsDown(KEYBOARD.W) then
		--AddVelocity(0, 0, 10, 0)
		EAddForce(10)
		--EToggle(3)
	end
	if KeyIsDown(KEYBOARD.S) then
		--EAddVelocity(0, -10, 0)
	end
	--Future: Update the collide to take tag
	
end

update()
