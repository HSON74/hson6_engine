
function update()
	if KeyIsDown(KEYBOARD.D) then
		EAddVelocity(20, 0, 0)
		SetScale(0, 3, 3, 0)
	end
	if KeyIsDown(KEYBOARD.A) then
		EAddVelocity(-20, 0, 0)
		SetScale(0, -3, 3, 0)
	end
	if KeyIsDown(KEYBOARD.W) then
		--AddVelocity(0, 0, 10, 0)
		Stop("Jump")
		--Play("Jump")
		PlayAnimation(0, "Jump")
		EAddForce(20)
		--EToggle(3)
	else
		PlayAnimation(0, "Idle")
	end
	if KeyIsDown(KEYBOARD.S) then
		--EAddVelocity(0, -10, 0)
	end
	--Future: Update the collide to take tag
	if Collide(0, "Coin") then
		--print("It hit")
		local z = CoinCollide(0, "Coin");
		--Play("Coin")
		Destroy(z);
	end
	if Collide(0, "Flag") then
		--print("It hit Flag")
		Stop("Win")

		--Play("Win")
	end
	
end

update()
