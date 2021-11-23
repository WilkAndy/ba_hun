/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/


#ifndef BASEFF1VG_H
#define BASEFF1VG_H

#include "RichardsSeff.h"
#include "RichardsSeffVG.h"

/**
 * Effective saturation as a function of porepressure
 * using the van Genuchten formula.  Note this is not a function
 * of capillary pressure: i use porepressure instead, so
 * seff = 1 for p - modflow_shift >= 0.
 */
class BASeff1VG : public RichardsSeff
{
public:
  BASeff1VG(const InputParameters & parameters);

  static InputParameters validParams();

  /**
   * effective saturation as a function of porepressure
   * @param p porepressure in the element.  Note that (*p[0])[qp] is the porepressure at quadpoint qp
   * @param qp the quad point to evaluate effective saturation at
   */
  Real seff(std::vector<const VariableValue *> p, unsigned int qp) const;

  /**
   * derivative of effective saturation as a function of porepressure
   * @param p porepressure in the element.  Note that (*p[0])[qp] is the porepressure at quadpoint qp
   * @param qp the quad point to evaluate effective saturation at
   * @param result the derivtives will be placed in this array
   */
  void dseff(std::vector<const VariableValue *> p, unsigned int qp, std::vector<Real> & result) const;

  /**
   * second derivative of effective saturation as a function of porepressure
   * @param p porepressure in the element.  Note that (*p[0])[qp] is the porepressure at quadpoint qp
   * @param qp the quad point to evaluate effective saturation at
   * @param result the derivtives will be placed in this array
   */
  void d2seff(std::vector<const VariableValue *> p, unsigned int qp, std::vector<std::vector<Real> > & result) const;

protected:

  /// van Genuchten alpha parameter
  Real _al;

  /// van Genuchten m parameter
  Real _m;

  /// modflow_shift
  Real _modflow_shift;

};

#endif // BASEFF1VG_H
