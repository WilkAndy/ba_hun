#include "BaHunApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"
#include "ModulesApp.h"

InputParameters BaHunApp::validParams()
{
  InputParameters params = MooseApp::validParams();

  params.set<bool>("use_legacy_uo_initialization") = false;
  params.set<bool>("use_legacy_uo_aux_computation") = false;
  params.set<bool>("use_legacy_output_syntax") = false;

  return params;
}

BaHunApp::BaHunApp(InputParameters parameters) :
    MooseApp(parameters)
{
  BaHunApp::registerAll(_factory, _action_factory, _syntax);
}

BaHunApp::~BaHunApp()
{
}

void
BaHunApp::registerAll(Factory & factory, ActionFactory & af, Syntax & syntax)
{
  ModulesApp::registerAll(factory, af, syntax);

  Registry::registerObjectsTo(factory, {"BaHunApp"});
  Registry::registerActionsTo(af, {"BaHunApp"});
}

void
BaHunApp::registerApps()
{
  registerApp(BaHunApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
BaHunApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  BaHunApp::registerAll(f, af, s);
}
extern "C" void
BaHunApp__registerApps()
{
  BaHunApp::registerApps();
}
