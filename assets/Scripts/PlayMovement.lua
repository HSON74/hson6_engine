
function update()
	if Collide(0, "Lava") then
		Destroy(0)
		Destroy(15)
	else
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
			Play("Jump")
			--PlayAnimation(0, "Jump")
			SetScale(0, 0, 0, 0)
			SetScale(15, 3, 3, 0)
			if KeyIsDown(KEYBOARD.A) then
				SetScale(15, -3, 3, 0)
			end
			EAddForce(10)
			--EToggle(3)
		else
			--PlayAnimation(0, "Idle")
			local x = GetSprite(0);
			SetScale(15, 0, 0, 0)
			SetScale(0, 3, 3, 0)
			if KeyIsDown(KEYBOARD.A) then
				SetScale(0, -3, 3, 0)
			end
			x.layer = 2
		end
		if KeyIsDown(KEYBOARD.S) then
			--EAddVelocity(0, -10, 0)
		end
		--Future: Update the collide to take tag
		if Collide(0, "Coin") then
			--print("It hit")
			local z = CoinCollide(0, "Coin");
			Stop("Coin")
			Play("Coin")
			Destroy(z);
		else
			
		end
		if Collide(0, "Flag") then
			--print("It hit Flag")
			Stop("Win")

			Play("Win")
		end
		if Collide(0, "Lava") then
			--print("It hit Flag")
			Quit();
		end
	end
	
end

update()
