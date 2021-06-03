--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE 2021/06/03 20:21:58
--

require "UnLua"

local TestLua_C = Class("TestLua_C")

--function TestLua_C:Initialize(Initializer)
--end

--function TestLua_C:UserConstructionScript()
--end

function TestLua_C:ReceiveBeginPlay()
	self.Overridden.ReceiveBeginPlay(self)
	print("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@")
end

--function TestLua_C:ReceiveEndPlay()
--	self.Overridden.ReceiveEndPlay(self)
--end

-- function TestLua_C:ReceiveTick(DeltaSeconds)
--	self.Overridden.ReceiveTick(self,DeltaSeconds)
-- end

--function TestLua_C:ReceiveAnyDamage(Damage, DamageType, InstigatedBy, DamageCauser)
--end

--function TestLua_C:ReceiveActorBeginOverlap(OtherActor)
--end

--function TestLua_C:ReceiveActorEndOverlap(OtherActor)
--end

return TestLua_C
