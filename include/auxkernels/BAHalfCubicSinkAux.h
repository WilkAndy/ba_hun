/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef BAHALFCUBICSINKAUX_H
#define BAHALFCUBICSINKAUX_H

#include "AuxKernel.h"

class Function;

/**
 * BAHalfCubicSinkAux allows visualisation of a
 * BAHalfCubicSink BC.  Measured in kg.m^-2.year^-1
 */
class BAHalfCubicSinkAux : public AuxKernel
{
public:
  BAHalfCubicSinkAux(const InputParameters & parameters);
  static InputParameters validParams();

  virtual ~BAHalfCubicSinkAux() {}

protected:
  virtual Real computeValue();

private:

  /// Denote x = porepressure - centre.  Then Flux out = (max/cutoff^3)*(2x + cutoff)(x - cutoff)^2 for cutoff < x < 0.  Flux out = max for x >= 0.  Flux out = 0 for x <= cutoff.
  Real _maximum;

  /// Denote x = porepressure - centre.  Then Flux out = (max/cutoff^3)*(2x + cutoff)(x - cutoff)^2 for cutoff < x < 0.  Flux out = max for x >= 0.  Flux out = 0 for x <= cutoff.
  const Function & _cutoff;

  /// Denote x = porepressure - centre.  Then Flux out = (max/cutoff^3)*(2x + cutoff)(x - cutoff)^2 for cutoff < x < 0.  Flux out = max for x >= 0.  Flux out = 0 for x <= cutoff.
  Real _centre;

  /// The porepressure
  const VariableValue & _pressure_var;

  /// the multiplier function: the flux out will be multiplied by this function
  const Function & _m_func;
};

#endif //BAHALFCUBICSINKAUX_H
