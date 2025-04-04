[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 1
  ny = 1
  xmin = 0
  xmax = 1
  ymin = 0
  ymax = 1
[]


[GlobalParams]
  richardsVarNames_UO = PPNames
[]

[UserObjects]
  [./PPNames]
    type = RichardsVarNames
    richards_vars = pressure
  [../]
  [./DensityConstBulk]
    type = RichardsDensityConstBulk
    dens0 = 1
    bulk_mod = 1
  [../]
  [./SeffVG]
    type = RichardsSeff1VG
    m = 0.5
    al = 1
  [../]
  [./RelPermPower]
    type = RichardsRelPermPower
    simm = 0.0
    n = 2
  [../]
  [./Saturation]
    type = RichardsSat
    s_res = 0.1
    sum_s_res = 0.2
  [../]
  [./SUPGstandard]
    type = RichardsSUPGstandard
    p_SUPG = 0.1
  [../]
[]

[Variables]
  [./pressure]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = FunctionIC
      function = initial_pressure
    [../]
  [../]
[]

[AuxVariables]
  [./seff]
  [../]
  [./one_hundred]
  [../]
[]


[Functions]
  [./initial_pressure]
    type = ParsedFunction
    value = 2
  [../]

  [./mass_bal_fcn]
    type = ParsedFunction
    value = abs((mi-lfout-rfout-mf)/2/(mi+mf))
    vars = 'mi mf lfout rfout'
    vals = 'mass_init mass_fin left_flux_out right_flux_out'
  [../]
[]

[Postprocessors]
  [./mass_init]
    type = RichardsMass
    variable = pressure
    execute_on = timestep_begin
  [../]
  [./mass_fin]
    type = RichardsMass
    variable = pressure
    execute_on = timestep_end
  [../]
  [./left_flux_out]
    type = BAPiecewiseLinearSinkFlux
    boundary = left
    variable = pressure
    pressures = '-1 1'
    bare_fluxes = '1E2 2E2'
    use_mobility = true
    use_relperm = true
    aux_var = one_hundred
  [../]
  [./right_flux_out]
    type = BAPiecewiseLinearSinkFlux
    boundary = right
    variable = pressure
    pressures = '-1 1'
    bare_fluxes = '1E2 2E2'
    use_mobility = true
    use_relperm = true
    aux_var = -10
  [../]
  [./p0]
    type = PointValue
    point = '0 0 0'
    variable = pressure
  [../]
  [./s0]
    type = PointValue
    point = '0 0 0'
    variable = seff
  [../]
  [./mass_bal]
    type = FunctionValuePostprocessor
    function = mass_bal_fcn
  [../]
[]

[BCs]
  [./left_flux]
    type = BAPiecewiseLinearSink
    boundary = left
    pressures = '-1 1'
    bare_fluxes = '1 2'
    variable = pressure
    use_mobility = true
    use_relperm = true
    aux_var = one_hundred
  [../]
  [./right_flux]
    type = BAPiecewiseLinearSink
    boundary = right
    pressures = '-1 1'
    bare_fluxes = '-1E1 -2E1'
    variable = pressure
    use_mobility = true
    use_relperm = true
    aux_var = -10
  [../]
[]

[Kernels]
  active = 'richardst'
  [./richardst]
    type = RichardsMassChange
    variable = pressure
  [../]
[]

[AuxKernels]
  [./seff_auxk]
    type = RichardsSeffAux
    variable = seff
    seff_UO = SeffVG
    pressure_vars = 'pressure'
  [../]
  [./one_hundred]
    type = ConstantAux
    variable = one_hundred
    value = 100
  [../]
[]

[Materials]
  [./rock]
    type = RichardsMaterial
    block = 0
    mat_porosity = 0.1
    mat_permeability = '1E-5 2.1E-5 2.2E-5  2.1E-5 0.1E-5 3.3E-5  2.2E-5 3.3E-5 2E-5'
    density_UO = DensityConstBulk
    relperm_UO = RelPermPower
    SUPG_UO = SUPGstandard
    sat_UO = Saturation
    seff_UO = SeffVG
    viscosity = 1E-3
    gravity = '-1 0 0'
    linear_shape_fcns = true
  [../]
[]


[Preconditioning]
  [./andy]
    type = SMP
    full = true
    #petsc_options = '-snes_test_display'
    petsc_options_iname = '-ksp_type -pc_type -snes_atol -snes_rtol -snes_max_it'
    petsc_options_value = 'bcgs bjacobi 1E-12 1E-10 10000'
  [../]
[]

[Executioner]
  type = Transient
  solve_type = Newton
  dt = 2E-3
  end_time = 0.2
[]

[Outputs]
  execute_on = 'timestep_end'
  file_base = s03
  csv = true
[]
