
PV_PULSE_LIST parameter
{
  display_name "Excitation Pulse Shape";
  relations    ExcPulseEnumRelation;
}ExcPulseEnum;

PVM_RF_PULSE parameter
{
  display_name "Excitation Pulse";
  relations    ExcPulRelation;
}ExcPul;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "Excitation Pulse Amplitude";
  relations ExcPulseAmplRel;
}ExcPulseAmpl;

double parameter
{
  editable false;
}ExcPulseShape[];


