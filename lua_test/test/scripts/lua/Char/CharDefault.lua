CharDefault={};

function CharDefault:OnLoad()

end

function CharDefault:OnCreate()

end

function CharDefault:OnDeath()

end

function CharDefault:OnResurrection(CasterID, ProducerType, ProducerID, BuffID, EventID, EffectID, ParamList)

end

function CharDefault:CaculateDamage(CasterID, ProducerType, ProducerID, BuffID, EventID, EffectID, ParamList)
	local DmgInfo=DamageInfo.new();

	DmgInfo.CasterID=CasterID;
	DmgInfo.ProducerType=ProducerType;
	DmgInfo.ProducerID=ProducerID;
	DmgInfo.BuffID=BuffID;
	DmgInfo.EventID=EventID;
	DmgInfo.EffectID=EffectID;

	return DmgInfo;
end

function CharDefault:OnDamage(DmgInfo)

end

function CharDefault:IsDied()
	return false;
end
