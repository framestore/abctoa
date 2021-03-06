//-*****************************************************************************
//
// Copyright (c) 2009-2010,
//  Sony Pictures Imageworks, Inc. and
//  Industrial Light & Magic, a division of Lucasfilm Entertainment Company Ltd.
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Sony Pictures Imageworks, nor
// Industrial Light & Magic nor the names of their contributors may be used
// to endorse or promote products derived from this software without specific
// prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//-*****************************************************************************

#ifndef _AlembicHolder_DrawContext_h_
#define _AlembicHolder_DrawContext_h_

#include "Foundation.h"
#include "parseJson.h"

namespace AlembicHolder {

//-*****************************************************************************
class DrawContext
{
public:
    DrawContext()
    {
        m_localToWorld.makeIdentity();
        m_worldToCamera.makeIdentity();
        m_pointSize = 3.0f;
    }

    // Default copy & assign.

    // Get/Set local-to-world
    const M44d &getLocalToWorld() const { return m_localToWorld; }
    void setLocalToWorld( const M44d & iXf ) { m_localToWorld = iXf; }

    // Get/Set world-to-camera
    const M44d &getWorldToCamera() const { return m_worldToCamera; }
    void setWorldToCamera( const M44d & iXf ) { m_worldToCamera = iXf; }

    // Get/Set point size
    float getPointSize() const { return m_pointSize; }
    void setPointSize( float iPs ) { m_pointSize = iPs; }

    // Get/Sec selection
    const std::string &getSelection() const {return m_selectionPath;}
    void setSelection( const std::string & path) {m_selectionPath = path;}

    // for coloring
    void setShaderColors(const std::map<std::string, MColor> color) {m_shaderColors = color;}
    const std::map<std::string, MColor> getShaderColors() const {return m_shaderColors;}

    void setNormalFlipped(bool flippedNormal) {m_normalFlipped = flippedNormal;}
    const bool &isNormalFlipped() const {return m_normalFlipped;}

	// scene params
	void setParams(holderPrms* params) { m_params = params;};
	holderPrms* getParams() const { return m_params; };

protected:
    M44d m_localToWorld;
    M44d m_worldToCamera;
    float m_pointSize;
    std::string m_selectionPath;
    std::map<std::string, MColor> m_shaderColors;
    bool m_normalFlipped;
	holderPrms* m_params;
};

} // End namespace AlembicHolder

#endif
