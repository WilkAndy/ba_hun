/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/


#ifndef BAPLOTQUANTITY_H
#define BAPLOTQUANTITY_H

#include "GeneralPostprocessor.h"

class BAQuantity;

/**
 * Extracts the value from BAQuantity userobject
 */
class BAPlotQuantity : public GeneralPostprocessor
{
public:
  BAPlotQuantity(const InputParameters & parameters);
  static InputParameters validParams();

  virtual ~BAPlotQuantity();

  virtual void initialize();
  virtual void execute();

  /// returns the value of the BAQuantity
  virtual PostprocessorValue getValue();

protected:

  /// the BAQuantity userobject
  const BAQuantity & _total_value;
};


#endif /* BAPLOTQUANTITY_H */
