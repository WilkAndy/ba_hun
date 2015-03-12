# With perm changes
[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 2
  xmin = 0
  xmax = 2
  ny = 2
  ymin = 0
  ymax = 2
[]


[GlobalParams]
  richardsVarNames_UO = PPNames
  density_UO = DensityConstBulk
  relperm_UO = RelPermPower
  SUPG_UO = SUPGnone
  sat_UO = Saturation
  seff_UO = SeffVG
  viscosity = 1E-3
[]

[UserObjects]
  [./PPNames]
    type = RichardsVarNames
    richards_vars = pressure
  [../]
  [./DensityConstBulk]
    type = RichardsDensityConstBulk
    dens0 = 1
    bulk_mod = 1.0E3
  [../]
  [./SeffVG]
    type = RichardsSeff1VG
    m = 0.8
    al = 1 # same deal with PETSc's "constant state"
  [../]
  [./RelPermPower]
    type = RichardsRelPermPower
    simm = 0.0
    n = 2
  [../]
  [./Saturation]
    type = RichardsSat
    s_res = 0.1
    sum_s_res = 0.1
  [../]
  [./SUPGnone]
    type = RichardsSUPGnone
  [../]
[]

[Variables]
  [./pressure]
  [../]
[]

[ICs]
  [./pressure]
    type = ConstantIC
    variable = pressure
    value = 1
  [../]
[]


[Kernels]
  [./richardst]
    type = RichardsMassChange
    variable = pressure
  [../]
[]

[AuxVariables]
  [./i_zone]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./ch_zone]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./depth]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./kxx]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./kxy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./kxz]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./kyy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./kyz]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./kzz]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./i_zone]
    type = ConstantAux
    variable = i_zone
    value = 0
    execute_on = initial
  [../]
  [./ch_zone]
    type = FunctionAux
    variable = ch_zone
    function = 'x+y'
    execute_on = initial
  [../]
  [./depth]
    type = FunctionAux
    variable = depth
    function = '0'
    execute_on = initial
  [../]
  [./kxx]
    type = RealTensorValueAux
    variable = kxx
    tensor = permeability
    index_i = 0
    index_j = 0
  [../]
  [./kxy]
    type = RealTensorValueAux
    variable = kxy
    tensor = permeability
    index_i = 0
    index_j = 1
  [../]
  [./kxz]
    type = RealTensorValueAux
    variable = kxz
    tensor = permeability
    index_i = 0
    index_j = 2
  [../]
  [./kyy]
    type = RealTensorValueAux
    variable = kyy
    tensor = permeability
    index_i = 1
    index_j = 1
  [../]
  [./kyz]
    type = RealTensorValueAux
    variable = kyz
    tensor = permeability
    index_i = 1
    index_j = 2
  [../]
  [./kzz]
    type = RealTensorValueAux
    variable = kzz
    tensor = permeability
    index_i = 2
    index_j = 2
  [../]
[]

[Materials]
  [./rock]
    type = BAMaterial
    block = 0
    mat_porosity = 0.5
    mat_permeability = '0 0 0  0 0 0  0 0 0'
    gravity = '0 0 0'
    insitu_perm_zone = i_zone
    kh = '1'
    kv = '1'

    depth = depth
    decayh = 1
    decayv = -2

    change_perm_zone = ch_zone
    change_kh = '0 0 1+t 1'
    change_kv = '0 2 1 -1-t'
    linear_shape_fcns = true
  [../]
[]


[Preconditioning]
  [./andy]
    type = SMP
    full = true
    #petsc_options = '-snes_test_display'
    petsc_options_iname = '-ksp_type -pc_type -snes_atol -snes_rtol -snes_max_it'
    petsc_options_value = 'bcgs bjacobi 1E-15 1E-10 10000'
  [../]
[]

[Executioner]
  type = Transient
  start_time = -1
  end_time = 1
  dt = 1
  solve_type = Newton
[]

[Outputs]
  file_base = zones03
  output_initial = true
  output_final = true
  exodus = true
  print_perf_log = true
[]