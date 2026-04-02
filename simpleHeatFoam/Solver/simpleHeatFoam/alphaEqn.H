
    //Dbturb = turbulence->nut()/SchmitTurb;

    //Dbturb.correctBoundaryConditions();

            fvScalarMatrix alphaEqn
           ( 
	fvm::div(phi,alpha)
      - fvm::laplacian(Db,alpha)
               //- fvm::laplacian(Dbturb,alpha)
      - fvc::laplacian(DT,T)
	//+ fvc::laplacian((Db)*(alpha/mag(H))*zeta*L_zeta,mag(H))
            );
    alphaEqn.relax();
    alphaEqn.solve();
//******************mp Equation *********************************************************** 
