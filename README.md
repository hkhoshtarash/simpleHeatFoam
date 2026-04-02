# simpleHeatFoam

`simpleHeatFoam` is an OpenFOAM 7 steady-state pore-scale solver for incompressible nanofluid flow and heat transfer in porous media / open-cell metal foams.

The solver is built from `simpleFoam` and extends it by solving:
- momentum and pressure using the SIMPLE algorithm,
- a temperature equation,
- a nanoparticle volume-fraction equation (`alpha`) with optional activation,
- Brownian diffusion and temperature-related diffusion terms used in the Buongiorno-style formulation.

This repository corresponds to the solver used in the paper:

**Khoshtarash, H., Siavashi, M., Ramezanpour, M., & Blunt, M. J. (2023). _Pore-scale analysis of two-phase nanofluid flow and heat transfer in open-cell metal foams considering Brownian motion_. Applied Thermal Engineering, 221, 119847.** https://doi.org/10.1016/j.applthermaleng.2022.119847

## Features

- OpenFOAM 7 custom solver
- Steady-state incompressible formulation
- Heat transfer equation for `T`
- Nanoparticle concentration / volume fraction equation for `alpha`
- Brownian diffusion coefficient computed inside the solver
- Suitable for pore-scale studies in open-cell metal foams and related porous geometries

## Repository structure

Suggested layout:

```text
simpleHeatFoam/
│       ├── simpleHeatFoam.C
│       ├── createFields.H
│       ├── UEqn.H
│       ├── pEqn.H
│       ├── TEqn.H
│       ├── alphaEqn.H
│       └── Make/
├── tutorials/
│   └── caseName/
│       ├── 0/
│       ├── constant/
│       └── system/
├── figures/
├── related_papers/
│   └── Khoshtarash_et_al_2023.pdf
├── README.md
├── CITATION.cff
├── .gitignore
└── LICENSE
```

## Governing model

The solver follows the pore-scale framework described in the paper and is based on a finite-volume implementation in OpenFOAM. The paper states that the continuity, momentum, nanoparticle distribution, and energy equations are solved using OpenFOAM, with SIMPLE for pressure–velocity coupling.

## Required fields and properties

### Fields
A typical case should provide at least these fields in the `0/` directory:
- `U`
- `p`
- `T`
- `alpha`

### `constant/transportProperties`
The solver reads the following properties:
- `rho_f`
- `mu_f`
- `c_f`
- `k_f`
- `rho_p`
- `c_p`
- `k_p`
- `dp`
- `alphaRef`
- `k_Boltzman`
- `lambda`
- `alphaEqActivation`

## Compilation

From the solver directory:

```bash
wmake
```

If you want to compile in your OpenFOAM user applications directory, place the solver under:

```bash
$WM_PROJECT_USER_DIR/applications/solvers/simpleHeatFoam
```

and then run:

```bash
wmake
```

## Running a case

After compiling, move to a tutorial case and run:

```bash
simpleHeatFoam
```

A typical workflow is:

```bash
blockMesh
checkMesh
simpleHeatFoam
```

If the geometry and mesh come from an external workflow, use the mesh-generation / import steps required for that case before launching the solver.

## Notes

- `alphaEqActivation` can be used to turn the `alpha` equation on or off.
- The current uploaded source contains temporary macOS metadata (`__MACOSX`) and OpenFOAM build artifacts under `Make/linux64GccDPInt32Opt/`; these should not be committed to GitHub.
- It is better to upload a cleaned version of the solver source only.

## Citation

If you use this solver, please cite:

Khoshtarash, H., Siavashi, M., Ramezanpour, M., & Blunt, M. J. (2023). *Pore-scale analysis of two-phase nanofluid flow and heat transfer in open-cell metal foams considering Brownian motion*. *Applied Thermal Engineering, 221*, 119847. https://doi.org/10.1016/j.applthermaleng.2022.119847

## License

Choose a license before making the repository public.

- If you want it to align with common OpenFOAM-style redistribution, use **GPL-3.0**.
- If you want a simpler permissive license, use **MIT**.

For a solver derived from OpenFOAM source structure, **GPL-3.0** is usually the safer choice.
