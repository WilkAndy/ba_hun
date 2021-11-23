/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/


#ifndef BAQUANTITY_H
#define BAQUANTITY_H

#include "GeneralUserObject.h"

/**
 * Records a quantity into _quant
 * This is used, for instance, to record the porepressure at a point
 *
 * This is a suboptimal setup because it requires a const_cast
 * of a BAQuantity object in order to do the recording
 */
class BAQuantity : public GeneralUserObject
{
public:
  BAQuantity(const InputParameters & parameters);
  static InputParameters validParams();

  virtual ~BAQuantity();

  /// sets _quant = 0
  void zero();

  /**
   * records contrib into _quant
   */
  void record(Real contrib);

  /// does nothing
  virtual void initialize();

  /// does nothing
  virtual void execute();

  /// does MPI stuff on _quant
  virtual void finalize();

  /// returns _quant
  virtual Real getValue() const;

protected:

  /// this holds the quantity
  Real _quant;
};

#endif /* BAQUANTITY_H */
