
function start()
	CreateObject("Mark", "Plumber.png") --Entity 0
	local x = GetSprite(0);
	x.layer = 2
    SetScale(0, 3, 3, 0)
   -- LoadSound("Background", "Music.wav");
    LoadSound("Jump", "Jump.wav");
    LoadSound("Coin", "Coin.wav");
    LoadSound("Win", "Win.wav");
    --Play("Background")
	AddScript(0, "PlayMovement")
	ECreateObject("Grass", "Grass.jpg") --Entity 1
    ECreateObject("Grass", "Grass.jpg") --Entity 2
    ECreateObject("Dirt", "Dirt.jpg") --Entity 3
    ECreateObject("Dirt", "Dirt.jpg") --Entity 4
    ECreateObject("Grass", "Grass.jpg") --Entity 5
    ECreateObject("Grass", "Grass.jpg") --Entity 6
    ECreateObject("Dirt", "Dirt.jpg") --Entity 7
    ECreateObject("Dirt", "Dirt.jpg") --Entity 8
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
    SetPosition(9, 3300, 600, 0)
    SetPosition(10, -2700, 0, 0)
    ECreateObject("Mario", "Mario.jpeg")
    local y = GetSprite(11)
    y.tag = "Background"
    y.layer = 12
    SetScale(11, 100, 100, 2)
    ECreateObject("Coin", "Coin.png")
    local y1 = GetSprite(12)
    y1.tag = "Coin"
    y1.layer = 11
    setActiveTrigger(12, true)
    SetPosition(12, 1000, 600, 0)
    ECreateObject("Coin", "Coin.png")
    local y1 = GetSprite(13)
    y1.tag = "Coin"
    y1.layer = 11
    setActiveTrigger(13, true)
    SetPosition(13, 2000, 600, 0)
    ECreateObject("Coin", "Coin.png")
    local y1 = GetSprite(14)
    y1.tag = "Coin"
    y1.layer = 11
    setActiveTrigger(14, true)
    SetPosition(14, 3000, 600, 0);
    CreateAnimator(0, "UIPlayer", "UIText");
    LoopAnimation(0, "UIPlayer", true)
    PlayAnimation(0, "UIPlayer")
    CreateAnimatorIdle(0, "Idle", "Plumber.png")
    CreateAnimatorIdle(0, "Jump", "JumpMan.png")
    
    
end

function update() 
end

start()
update()