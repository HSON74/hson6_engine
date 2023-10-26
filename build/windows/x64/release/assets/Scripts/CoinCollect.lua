
--end

function start()
	CreateObject("Hello", "Image.png") --Entity 0
	local x = GetSprite(0);
	x.layer = 0
	AddScript(0, "PlayMovement")
	CreateObject("Coin", "Coin.png")
	CreateObject("Coin", "Coin.png")
	CreateObject("Coin", "Coin.png")
	CreateObject("Coin", "Coin.png")
	--CreateObject("Coin", "Coin.png")
	--CreateObject("Coin", "Coin.png")
	--SetScale(1, 5, 5)
	--SetPosition(5, 10, 0, 0)
	--SetPosition(6, 20, 0, 0)
	SetPosition(1, 50, 0, 0)
	SetPosition(2, 0, -50, 0)
	SetPosition(3, 0, 100, 0)
	SetPosition(4,-100, 0, 0)
end

function update() 
end

print("problem start")
start()
update()
print("problem start end")