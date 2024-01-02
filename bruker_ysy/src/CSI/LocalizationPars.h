LOCALIZATION_MODE parameter
{
  display_name "Localization Mode";
  relations  backbone;
}LocalizationMode;

/*RF Pulses*/
PV_PULSE_LIST parameter
{
  display_name "Refocusing Pulse Shape";
  relations    RefPulse1EnumRelation;
}RefPulse1Enum;

PVM_RF_PULSE parameter
{
  display_name "Refocusing Pulse";
  relations    RefPulse1Relation;
}RefPulse1;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "RF Pulse Amplitude";
  relations RefPulse1AmplRel;
}RefPulse1Ampl;

double parameter
{
  relations backbone;
}RefPulse1Shape[];


/*Spoiler*/
PVM_SPOILER_TYPE parameter 
{
  display_name "Refocusing Spoiler";
  relations RefSpoilerRel;
}RefSpoiler;

double parameter RefSpAmp;
double parameter RefSpDur;
double parameter RefGrad1;
double parameter RefGrad2;
double parameter FillTE1_1;
double parameter FillTE1_2;
double parameter FillTE2_1;
double parameter FillTE2_2;
double parameter MinTE1_1;
double parameter MinTE1_2;
double parameter MinTE2_1;
double parameter MinTE2_2;

