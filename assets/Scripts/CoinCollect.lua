
--end

function start()
	CreateObject("Hello", "Image.png") --Entity 0
	local x = GetSprite(0);
	x.layer = 0
	AddScript(0, "PlayMovement")
	CreateObject("Coin", "Coin.png")
	--CreateObject("Coin", "Coin.png")
	--CreateObject("Coin", "Coin.png")
	--CreateObject("Coin", "Coin.png")
	AddRigidBody(0)
	--setActiveGravity(0,false)
	--CreateObject("Coin", "Coin.png")
	CreateObject("Coin", "Coin.png")
	--SetScale(1, 5, 5)
	--SetPosition(5, 10, 0, 0)
	--SetPosition(6, 20, 0, 0)
	SetPosition(1, 0, -400, 0)
	SetPosition(2, 400, 0, 0)
	--SetPosition(3, 0, -200, 0)
	--SetPosition(4,-200, 0, 0)
	--SetPosition(1, 200, 0, 0)
	--SetPosition(2, 0, 200, 0)
	--SetPosition(3, 0, -200, 0)
	--SetPosition(4,-200, 0, 0)
end

function update() 
end

print("problem start")
start()
update()
print("problem start end")