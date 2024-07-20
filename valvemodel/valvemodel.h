#pragma once

enum eDeviceType {
    TRIODE,
    PENTODE
};

/**
 * @brief The eTriodeParameter enum
 *
 * Defines indexes into the array of model Parameters. This is useful because the models considered
 * represent progressive refinements, i.e. the more complex models reuse parameters from the simpler
 * models and complex models only need to define the additional parameters that they add to the model
 */
enum eParameter {
    PAR_MU,
    PAR_KG1,
    PAR_X,
    PAR_KP,
    PAR_KVB,
    PAR_KVB1,
    PAR_VCT,
    PAR_KG2,
    PAR_KG2A,
    PAR_A,
    PAR_ALPHA,
    PAR_BETA,
    PAR_GAMMA,
    PAR_OS,
    PAR_TAU,
    PAR_RHO,
    PAR_THETA,
    PAR_PSI,
    PAR_OMEGA,
    PAR_LAMBDA,
    PAR_NU,
    PAR_S,
    PAR_AP
};

