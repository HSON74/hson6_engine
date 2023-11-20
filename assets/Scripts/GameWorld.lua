
function start()
	CreateObject("Mark", "Plumber.png") --Entity 0
	local x = GetSprite(0);
	x.layer = 2
    SetScale(0, 3, 3, 0)
	AddScript(0, "PlayMovement")
	ECreateObject("Grass", "Grass.jpg")
    ECreateObject("Grass", "Grass.jpg")
    ECreateObject("Dirt", "Dirt.jpg")
    ECreateObject("Dirt", "Dirt.jpg")
    ECreateObject("Grass", "Grass.jpg")
    ECreateObject("Grass", "Grass.jpg")
    ECreateObject("Dirt", "Dirt.jpg")
    ECreateObject("Dirt", "Dirt.jpg")
    ECreateObject("Flag", "Flag.png")
    ECreateObject("Coin", "Coin.png")
    local z = GetSprite(9)
    z.layer = 11
    setActiveTrigger(9, true)
    local a = GetSprite(10)
    a.layer = 11
    a.tag = "Coin"
    setActiveTrigger(10, true)
    SetScale(1, 12, 3, 0)
    SetScale(2, 12, 3, 0)
    SetScale(3, 12, 12, 0)
    SetScale(4, 12, 12, 0)
    SetScale(5, 12, 3, 0)
    SetScale(6, 12, 3, 0)
    SetScale(7, 12, 12, 0)
    SetScale(8, 12, 12, 0)
    SetScale(9, 3, 3, 0)
    SetScale(10, 3, 3, 0)
	SetPosition(1, -900, -600, 0)
	SetPosition(2, 1500, 0, 0)
    SetPosition(3, -900, -1500, 0)
    SetPosition(4, 1500, -1200, 0)
    SetPosition(5, -2700, -600, 0)
    SetPosition(6, 3300, 0, 0)
    SetPosition(7, -2700, -1500, 0)
    SetPosition(8, 3300, -1200, 0)
    SetPosition(9, 3300, 600, 0)
    SetPosition(10, -2700, 0, 0)
    ECreateObject("Mario", "Mario.jpeg")
    local y = GetSprite(11)
    y.tag = "Background"
    y.layer = 12
    SetScale(11, 10, 10, 0)
end

function update() 
end

start()
update()