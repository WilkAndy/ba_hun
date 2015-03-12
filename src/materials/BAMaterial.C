/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/


#include "BAMaterial.h"



template<>
InputParameters validParams<BAMaterial>()
{
  InputParameters params = validParams<RichardsMaterial>();
  params.addRequiredCoupledVar("insitu_perm_zone", "An auxillary variable the defines the zone for insitu permeability.  For each element, the zone must be a nonnegative integer");
  params.addRequiredParam<std::vector<Real> >("kh", "A vector of horizontal insitu permeability values.  For insitu_perm_zone=0, the first of these is used.  For insitu_perm_zone=1, the second is used.  Etc.  You must ensure that for each insitu_perm_zone there is a value!");
  params.addRequiredParam<std::vector<Real> >("kv", "A vector of vertical insitu permeability values.  For insitu_perm_zone=0, the first of these is used.  For insitu_perm_zone=1, the second is used.  Etc.  You must ensure that for each insitu_perm_zone there is a value!");

  params.addCoupledVar("depth", 0, "An auxillary variable the defines depth of an element.  This is used to modify the insitu permeabilities.  If not given then it defaults to zero");
  params.addParam<Real>("decayh", 0.0, "The insitu horizontal permeabilities will be multiplied by Exp(-decayh*depth)");
  params.addParam<Real>("decayv", 0.0, "The insitu vertical permeabilities will be multiplied by Exp(-decayv*depth)");

  params.addRequiredCoupledVar("change_perm_zone", "An auxillary variable the defines the zone for changes in permeability.  For each element, the zone must be a nonnegative integer");
  params.addRequiredParam<std::vector<FunctionName> >("change_kh", "A vector of changes in horizontal permeability.  For change_perm_zone=0, the first of these is used.  For change_perm_zone=1, the second is used.  Etc. permeability = kh*Exp(-decayh*depth)*10**change_kh.   You must ensure that for each change_perm_zone there is a value!");
  params.addRequiredParam<std::vector<FunctionName> >("change_kv", "A vector of changes in vertical permeability.  For change_perm_zone=0, the first of these is used.  For change_perm_zone=1, the second is used.  Etc. permeability = kv*Exp(-decayv*depth)*10**change_kv.   You must ensure that for each change_perm_zone there is a value!");

  params.addClassDescription("Material designed to work well with the BA project which has a lot of different zones, so is not suitable for dividing into blocks");
  return params;
}

BAMaterial::BAMaterial(const std::string & name, InputParameters parameters) :
    RichardsMaterial(name, parameters),
    _insitu_perm_zone(coupledValue("insitu_perm_zone")),
    _kh(getParam<std::vector<Real> >("kh")),
    _kv(getParam<std::vector<Real> >("kv")),

    _depth(coupledValue("depth")),
    _decayh(getParam<Real>("decayh")),
    _decayv(getParam<Real>("decayv")),

    _change_perm_zone(coupledValue("change_perm_zone"))
{
  if (_kh.size() != _kv.size())
    mooseError("BAMaterial: For clarity kh and kv must have the same size\n");

  const std::vector<FunctionName> & fcn_namesh(getParam<std::vector<FunctionName> >("change_kh"));
  const std::vector<FunctionName> & fcn_namesv(getParam<std::vector<FunctionName> >("change_kv"));

  const unsigned num = fcn_namesh.size();
  if (num != fcn_namesv.size())
    mooseError("BAMaterial: For clarity change_kh and change_kv must have the same size\n");

  _change_kh.resize(num);
  _change_kv.resize(num);
  for (unsigned i = 0 ; i < num ; ++i)
  {
    _change_kh[i] = &getFunctionByName(fcn_namesh[i]);
    _change_kv[i] = &getFunctionByName(fcn_namesv[i]);
  }
}

void
BAMaterial::computeProperties()
{
  // compute porepressures and effective saturations
  // with algorithms depending on the _richards_name_UO.var_types()
  computePandSeff();

  // the insitu_perm_zone is assumed to be the same for each qp in the element
  if (_insitu_perm_zone[0] < 0)
    mooseError("BAMaterial: insitu_perm_zone is " << _insitu_perm_zone[0] << " which is negative!\n");
  unsigned int i_zone = (unsigned) _insitu_perm_zone[0] + 0.5;
  if (i_zone >= _kh.size())
    mooseError("BAMaterial: insitu_perm_zone is " << i_zone << " which is not smaller than the size of kh!\n");

  // the change_perm_zone is assumed to be the same for each qp in the element
  if (_change_perm_zone[0] < 0)
    mooseError("BAMaterial: change_perm_zone is " << _change_perm_zone[0] << " which is negative!\n");
  unsigned int ch_zone = (unsigned) _change_perm_zone[0] + 0.5;
  if (ch_zone >= _change_kh.size())
    mooseError("BAMaterial: change_perm_zone is " << ch_zone << " which is not smaller than the size of change_kh!\n");


  // porosity, permeability, and gravity
  for (unsigned int qp = 0; qp < _qrule->n_points(); qp++)
  {
    _porosity[qp] = _material_por + (*_por_change)[qp];
    _porosity_old[qp] = _material_por + (*_por_change_old)[qp];

    _permeability[qp] = RealTensorValue();
    _permeability[qp](0, 0) = _permeability[qp](1, 1) = _kh[i_zone]*std::exp(-_decayh*_depth[qp])*std::pow(10, _change_kh[ch_zone]->value(_t, _q_point[qp]));
    _permeability[qp](2, 2) = _kv[i_zone]*std::exp(-_decayv*_depth[qp])*std::pow(10, _change_kv[ch_zone]->value(_t, _q_point[qp]));

    _gravity[qp] = _material_gravity;
  }


  // compute "derived" quantities -- those that depend on P and Seff --- such as density, relperm
  for (unsigned int qp = 0; qp < _qrule->n_points(); qp++)
    computeDerivedQuantities(qp);


  // compute certain second derivatives of the derived quantities
  // These are needed in Jacobian calculations if doing SUPG
  for (unsigned int qp = 0; qp < _qrule->n_points(); qp++)
    compute2ndDerivedQuantities(qp);


  // Now for SUPG itself
  for (unsigned int qp = 0; qp < _qrule->n_points(); qp++)
    zeroSUPG(qp);

  // the following saves computational effort if all SUPG is trivial
  bool trivial_supg = true;
  for (unsigned int i = 0; i < _num_p; ++i)
    trivial_supg = trivial_supg && (*_material_SUPG_UO[i]).SUPG_trivial();
  if (trivial_supg)
    return;
  else
    computeSUPG();

}