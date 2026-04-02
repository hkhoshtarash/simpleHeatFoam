////////////////////////////
    Db=k_Boltzman*T/(3*3.14*mu_f*dp); 
    DT=0.26*(k_f/(2*k_f+k_p))*(mu_f/rho_f)*(alpha/T);
    rho_m = alpha*rho_p+(1-alpha)*rho_f;
    rho=rho_m;
    rhoC_m=alpha*rho_p*c_p+(1-alpha)*rho_f*c_f;
    mu_m = mu_f/pow((1.0-alpha),2.5);
    mu=mu_m;
    k_m = ((k_p+(lambda-1)*k_f-(lambda-1)*(k_f-k_p)*alpha)/(k_p+(lambda-1)*k_f+(k_f-k_p)*alpha))*k_f;       
//////////////////////////////////

    // Momentum predictor

    MRF.correctBoundaryVelocity(U);

    tmp<fvVectorMatrix> tUEqn
    (
        fvm::div(phi, U)
      //+ MRF.DDt(U)
      //+ turbulence->divDevReff(U)
        - fvm::laplacian((mu_m/rho_m), U)
        - fvc::div((mu_m/rho_m)*dev(fvc::grad(U)().T()))
     ==
        fvOptions(U)
    );
    fvVectorMatrix& UEqn = tUEqn.ref();

    UEqn.relax();

    fvOptions.constrain(UEqn);

    if (simple.momentumPredictor())
    {
        solve(UEqn == -fvc::grad(p));

        fvOptions.correct(U);
    }
