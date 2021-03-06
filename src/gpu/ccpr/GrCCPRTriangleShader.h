/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrCCPRTriangleShader_DEFINED
#define GrCCPRTriangleShader_DEFINED

#include "ccpr/GrCCPRCoverageProcessor.h"

/**
 * Passes 1 & 2: Draw the triangle's conservative raster hull with a coverage of 1, then smooth the
 *               edges by drawing the conservative rasters of all 3 edges and interpolating from
 *               coverage=-1 on the outside to coverage=0 on the inside.
 */
class GrCCPRTriangleShader : public GrCCPRCoverageProcessor::Shader {
    WindHandling onEmitVaryings(GrGLSLVaryingHandler*, SkString* code, const char* position,
                                const char* coverage, const char* wind) override;
    void onEmitFragmentCode(GrGLSLPPFragmentBuilder*, const char* outputCoverage) const override;

    GrGLSLVarying fCoverageTimesWind{kHalf_GrSLType, GrGLSLVarying::Scope::kGeoToFrag};
};

/**
 * Pass 3: Touch up the corner pixels. Here we fix the simple distance-to-edge coverage analysis
 *         done previously so that it takes into account the region that is outside both edges at
 *         the same time.
 */
class GrCCPRTriangleCornerShader : public GrCCPRCoverageProcessor::Shader {
    void emitSetupCode(GrGLSLVertexGeoBuilder*, const char* pts, const char* repetitionID,
                       const char* wind, GeometryVars*) const override;
    WindHandling onEmitVaryings(GrGLSLVaryingHandler*, SkString* code, const char* position,
                                const char* coverage, const char* wind) override;
    void onEmitFragmentCode(GrGLSLPPFragmentBuilder* f, const char* outputCoverage) const override;

    GrShaderVar fAABoxMatrices{"aa_box_matrices", kFloat2x2_GrSLType, 2};
    GrShaderVar fAABoxTranslates{"aa_box_translates", kFloat2_GrSLType, 2};
    GrShaderVar fGeoShaderBisects{"bisects", kFloat2_GrSLType, 2};
    GrGLSLVarying fCornerLocationInAABoxes{kFloat2x2_GrSLType, GrGLSLVarying::Scope::kGeoToFrag};
    GrGLSLVarying fBisectInAABoxes{kFloat2x2_GrSLType, GrGLSLVarying::Scope::kGeoToFrag};
};

#endif
