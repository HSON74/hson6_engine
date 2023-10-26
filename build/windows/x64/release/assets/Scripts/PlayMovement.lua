
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
	if Collide(0, "Coin1") then
		Destroy("Coin1")
	end
	if Collide(0, "Coin") then
		Destroy("Coin2")
	end
	if Collide(0, "Coin") then
		Destroy("Coin3")
	end
	if Collide(0, "Coin") then
		Destroy("Coin4")
	end
end

update()
