Info<< "Reading thermophysical properties\n" << endl;

    IOdictionary transportProperties
    (
        IOobject
        (
            "transportProperties",
            runTime.constant(),
            mesh,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    );
////base fluid
    dimensionedScalar rho_f
    (
        transportProperties.lookup("rho_f")
    );
    dimensionedScalar mu_f
    (
        transportProperties.lookup("mu_f")
    );
    dimensionedScalar c_f
    (
    	transportProperties.lookup("c_f")
    );
    dimensionedScalar k_f
    (
        transportProperties.lookup("k_f")
    );
/////////nanoparticle
    dimensionedScalar rho_p
    (
        transportProperties.lookup("rho_p")
    );
    dimensionedScalar c_p
    (
    	transportProperties.lookup("c_p")
    );
    dimensionedScalar k_p
    (
        transportProperties.lookup("k_p")
    );
    dimensionedScalar dp
    (
    	transportProperties.lookup("dp")
    );
    dimensionedScalar alphaRef
    (
    	transportProperties.lookup("alphaRef")
    );
///////constants
    dimensionedScalar k_Boltzman
    (
    	transportProperties.lookup("k_Boltzman")
    );
    dimensionedScalar lambda
    (
    	transportProperties.lookup("lambda")
    );
    dimensionedScalar alphaEqActivation
    (
    	transportProperties.lookup("alphaEqActivation")
    );
/////////


Info<< "Reading field T\n" << endl;
volScalarField T
(
    IOobject
    (
        "T",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field p\n" << endl;
volScalarField p
(
    IOobject
    (
        "p",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field U\n" << endl;
volVectorField U
(
    IOobject
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

///////////////////////////
    Info<< "Reading field alpha\n" << endl;
    volScalarField alpha
    (
        IOobject
        (
            "alpha",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info<< "Reading field Db\n" << endl;
    volScalarField Db
    (
        IOobject
        (
            "Db",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("Db", dimensionSet(0,2,-1,0,0,0,0), 1e-10)
    );
    Db=k_Boltzman*T/(3*3.14*mu_f*dp);

    Info<< "Reading field DT\n" << endl;
    volScalarField DT
    (
        IOobject
        (
            "DT",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("DT", dimensionSet(0,2,-1,-1,0,0,0), 1e-10)
    );
    DT=0.26*(k_f/(2*k_f+k_p))*(mu_f/rho_f)*(alpha/T);

    Info<< "Reading field rho_m\n" << endl;
    volScalarField rho_m
    (
        IOobject
        (
            "rho_m",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("rho_m", dimMass/dimVolume, 1e-10)
    );
    rho_m = alpha*rho_p+(1-alpha)*rho_f;

    Info<< "Reading field rho\n" << endl;
    volScalarField rho
    (
        IOobject
        (
            "rho",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
       ),
        mesh,
        dimensionedScalar("rho", dimMass/dimVolume, 1e-10)
    );
    rho=rho_m;

    Info<< "Reading field rhoC_m\n" << endl;
    volScalarField rhoC_m
    (
        IOobject
        (
            "rhoC_m",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("rhoC_m", dimensionSet(1,-1,-2,-1,0,0,0), 1e-10)
    );
    rhoC_m=alpha*rho_p*c_p+(1-alpha)*rho_f*c_f;

    Info<< "Reading field mu_m\n" << endl;
    volScalarField mu_m
    (
        IOobject
        (
            "mu_m",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("mu_m", dimVelocity*dimLength*dimMass/dimVolume, 1e-10)
    );
    mu_m = mu_f/pow((1.0-alpha),2.5);

    Info<< "Reading field mu\n" << endl;
    volScalarField mu
    (
        IOobject
        (
            "thermo:mu",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("thermo:mu", dimVelocity*dimLength*dimMass/dimVolume, 1e-10)
    );
    mu=mu_m;

    Info<< "Reading field k_m\n" << endl;
    volScalarField k_m
    (
        IOobject
        (
            "k_m",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("k_m", dimensionSet(1,1,-3,-1,0,0,0), 1e-10)
    );
	k_m = ((k_p+(lambda-1)*k_f-(lambda-1)*(k_f-k_p)*alpha)/(k_p+(lambda-1)*k_f+(k_f-k_p)*alpha))*k_f;       
//////////////////////////////////
#include "createPhi.H"


label pRefCell = 0;
scalar pRefValue = 0.0;
setRefCell(p, simple.dict(), pRefCell, pRefValue);
mesh.setFluxRequired(p.name());


singlePhaseTransportModel laminarTransport(U, phi);

autoPtr<incompressible::turbulenceModel> turbulence
(
    incompressible::turbulenceModel::New(U, phi, laminarTransport)
);

#include "createMRF.H"
#include "createFvOptions.H"
