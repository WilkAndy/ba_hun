#ifndef BA_HUNAPP_H
#define BA_HUNAPP_H

#include "MooseApp.h"

class BaHunApp : public MooseApp
{
public:
  BaHunApp(InputParameters parameters);

  virtual ~BaHunApp();

  static InputParameters validParams();

  static void registerApps();
  static void registerAll(Factory & factory, ActionFactory & af, Syntax & syntax);
};

#endif /* BA_HUNAPP_H */
