--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

require "UnLua"

local TemplateName = Class("TemplateName")

--function TemplateName:Initialize(Initializer)
--end

--function TemplateName:UserConstructionScript()
--end

function TemplateName:ReceiveBeginPlay()
	self.Overridden.ReceiveBeginPlay(self)
end

--function TemplateName:ReceiveEndPlay()
--	self.Overridden.ReceiveEndPlay(self)
--end

-- function TemplateName:ReceiveTick(DeltaSeconds)
--	self.Overridden.ReceiveTick(self,DeltaSeconds)
-- end

--function TemplateName:ReceiveAnyDamage(Damage, DamageType, InstigatedBy, DamageCauser)
--end

--function TemplateName:ReceiveActorBeginOverlap(OtherActor)
--end

--function TemplateName:ReceiveActorEndOverlap(OtherActor)
--end

return TemplateName
