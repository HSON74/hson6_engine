
function start()
	CreateObject("Mark", "Plumber.png") --Entity 0
	local x = GetSprite(0);
	x.layer = 2
    SetScale(0, 3, 3, 0)
   LoadSound("Background", "Music.wav");
    LoadSound("Jump", "Jump.wav");
    LoadSound("Coin", "Coin.wav");
    LoadSound("Win", "Win.wav");
    Play("Background")
	AddScript(0, "PlayMovement")
	ECreateObject("Grass", "Grass.jpg") --Entity 1
    local grass = GetSprite(1)
    grass.layer = 4
    grass.tag = "Ground"
    ECreateObject("Grass", "Grass.jpg") --Entity 2
    grass = GetSprite(2)
    grass.layer = 4
    grass.tag = "Ground"
    ECreateObject("Dirt", "Dirt.jpg") --Entity 3
    grass = GetSprite(3)
    grass.tag = "Ground"
    ECreateObject("Dirt", "Dirt.jpg") --Entity 4
    grass = GetSprite(4)
    grass.tag = "Ground"
    ECreateObject("Grass", "Grass.jpg") --Entity 5
    grass = GetSprite(5)
    grass.layer = 4
    grass.tag = "Ground"
    ECreateObject("Grass", "Grass.jpg") --Entity 6
    grass = GetSprite(6)
    grass.layer = 4
    grass.tag = "Ground"
    ECreateObject("Dirt", "Dirt.jpg") --Entity 7
    grass = GetSprite(7)
    grass.tag = "Ground"
    ECreateObject("Dirt", "Dirt.jpg") --Entity 8
    grass = GetSprite(8)
    grass.tag = "Ground"
    ECreateObject("Flag", "Flag.png") --Entity 9
    ECreateObject("Coin", "Coin.png") --Entity 10
    local z = GetSprite(9)
    z.layer = 11
    z.tag = "Flag"
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
    
    SetPosition(10, -2700, 0, 0)
    ECreateObject("Mario", "Mario.jpeg")
    local y = GetSprite(11)
    y.tag = "Background"
    y.layer = 12
    SetScale(11, 1000, 100, 2)
    ECreateObject("Coin", "Coin.png")
    local y1 = GetSprite(12)
    y1.tag = "Coin"
    y1.layer = 11
    setActiveTrigger(12, true)
    
    ECreateObject("Coin", "Coin.png")
    local y1 = GetSprite(13)
    y1.tag = "Coin"
    y1.layer = 11
    setActiveTrigger(13, true)
    ECreateObject("Coin", "Coin.png")
    local y1 = GetSprite(14)
    y1.tag = "Coin"
    y1.layer = 11
    setActiveTrigger(14, true)
    CreateObject("Jump", "JumpMan.png")
    local jump = GetSprite(15)
    jump.layer = 9
    SetScale(15, 0, 0, 0)
    ECreateObject("Dirt", "Dirt.png") --Entity 16
    ECreateObject("Dirt", "Dirt.png") --Entity 17
    ECreateObject("Dirt", "Dirt.png") --Entity 18
    ECreateObject("Dirt", "Dirt.png") --Entity 19
    ECreateObject("Dirt", "Dirt.png") --Entity 20
    ECreateObject("Dirt", "Dirt.png") --Entity 21
    ECreateObject("Dirt", "Dirt.png") --Entity 22
    ECreateObject("Dirt", "Dirt.png") --Entity 23
    ECreateObject("Dirt", "Dirt.png") --Entity 24
    ECreateObject("Dirt", "Dirt.png") --Entity 25
    local dirt = GetSprite(16)
    dirt.tag = "Ground"
    dirt = GetSprite(17)
    dirt.tag = "Ground"
    dirt = GetSprite(18)
    dirt.tag = "Ground"
    dirt = GetSprite(19)
    dirt.tag = "Ground"
    dirt = GetSprite(20)
    dirt.tag = "Ground"
    dirt = GetSprite(21)
    dirt.tag = "Ground"
    dirt = GetSprite(22)
    dirt.tag = "Ground"
    dirt = GetSprite(23)
    dirt.tag = "Ground"
    dirt = GetSprite(24)
    dirt.tag = "Ground"
    dirt = GetSprite(25)
    dirt.tag = "Ground"
    SetScale(16, 3, 3, 0)
    SetScale(17, 3, 3, 0)
    SetScale(18, 3, 3, 0)
    SetScale(19, 3, 3, 0)
    SetScale(20, 3, 3, 0)
    SetScale(21, 3, 3, 0)
    SetScale(22, 3, 3, 0)
    SetScale(23, 3, 3, 0)
    SetScale(24, 3, 3, 0)
    SetScale(25, 3, 3, 0)
    SetPosition(16, 5000, 600, 0)
    SetPosition(17, 7200, 300, 0)
    SetPosition(18, 9500, 0, 0)
    SetPosition(19, 11550, -300, 0)
    SetPosition(20, 13000, 0, 0)
    SetPosition(21, 15000, 300, 0)
    SetPosition(22, 17000, 0, 0)
    SetPosition(23, 19000, -300, 0)
    SetPosition(24, 21500, 0, 0)
    SetPosition(25, 24000, 300, 0)
    SetPosition(9, 24000, 900, 0)
    SetPosition(12, 1000, 600, 0)
    SetPosition(13, 15000, 900, 0)
    SetPosition(14, 21500, 600, 0)
    ECreateObject("lava", "lava.jpg") --Entity 26
    local lava = GetSprite(26)
    lava.tag = "Lava"
    lava.layer = 1
    SetScale(26, 500, 10, 0)
    SetPosition(26, 0, -10000, 0)
    --CreateAnimator(0, "UIPlayer", "UIText");
    --LoopAnimation(0, "UIPlayer", true)
    --PlayAnimation(0, "UIPlayer")
    --CreateAnimatorIdle(0, "Idle", "Plumber.png")
    --CreateAnimatorIdle(0, "Jump", "JumpMan.png")
    
    
end

function update() 
    
end

start()
update()