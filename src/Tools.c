
void wobble(float *CurrentPosition, float TargetPosition, float Force,float Friction, float *Spd)
{
	*Spd=((*Spd)+((TargetPosition-(*CurrentPosition))/2.f)*Force)*(1.f-Friction);
	*CurrentPosition+=(*Spd);
}