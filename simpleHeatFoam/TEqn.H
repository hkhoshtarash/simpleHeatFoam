{
    //alphat = turbulence->nut()/Prt;
    //alphat.correctBoundaryConditions();

    //volScalarField alphaEff("alphaEff", turbulence->nu()/Pr + alphat);
    volVectorField gradT=fvc::grad(T);
    volVectorField Jp=-rho_p*DT*fvc::grad(T)-rho_p*Db*fvc::grad(alpha);
    fvScalarMatrix TEqn
    (
        fvm::div(phi, T)
      //- fvm::laplacian(alphaEff, T)
      - fvm::laplacian((k_m/(rhoC_m)),T)
      + (c_p/(rhoC_m))*(gradT&Jp)
     ==
        fvOptions(T)
    );

    TEqn.relax();

    fvOptions.constrain(TEqn);

    TEqn.solve();

}
