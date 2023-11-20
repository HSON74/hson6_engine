
--end

function start()
	CreateObject("Hello", "Image.png") --Entity 0
	local x = GetSprite(0);
	x.layer = 2
	AddScript(0, "PlayMovement")
	ECreateObject("Coin", "Coin.png")
	
	--CreateObject("Coin", "Coin.png")
	--CreateObject("Coin", "Coin.png")
	--CreateObject("Coin", "Coin.png")
	--AddRigidBody(0)
	
	--CreateObject("Coin", "Coin.png")
	ECreateObject("Coin", "Coin.png")
	--SetScale(1, 5, 5)
	--SetPosition(5, 10, 0, 0)
	--SetPosition(6, 20, 0, 0)
	--EsetActiveGravity(false)
	SetScale(2, 1, 100, 1)
	SetPosition(1, 0, -400, 0)
	SetPosition(2, 400, 0, 0)
	CreateUI("first_number");
	UIUpdate(3, "0");
	CreateUI("Second_number");
	UIUpdate(4, "0");
	CreateUI("Third_number");
	UIUpdate(5, "0");
	SetPosition(3, -1100, 700, 0);
	SetPosition(4, -900, 700, 0);
	SetPosition(5, -700, 700, 0);
	ECreateObject("Bar", "Red_Box.png")
	setActiveTrigger(6, true)
	SetPosition(6, 0, 200, 0)

end

function update() 
end

print("problem start")
start()
update()
print("problem start end")