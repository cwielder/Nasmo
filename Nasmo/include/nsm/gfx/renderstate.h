#pragma once

#include <nsm/gfx/opengl.h>
#include <nsm/debug/assert.h>

namespace nsm {

    class RenderState final {
    public:
        enum class CullFace {
            Front = GL_FRONT,
            Back = GL_BACK,
            FrontAndBack = GL_FRONT_AND_BACK
        };

        enum class CullDirection {
            Clockwise = GL_CW,
            CounterClockwise = GL_CCW
        };

        enum class DepthFunction {
            Never = GL_NEVER,
            Less = GL_LESS,
            Equal = GL_EQUAL,
            LessEqual = GL_LEQUAL,
            Greater = GL_GREATER,
            NotEqual = GL_NOTEQUAL,
            GreaterEqual = GL_GEQUAL,
            Always = GL_ALWAYS
        };

        enum class BlendFactor {
            Zero = GL_ZERO,
            One = GL_ONE,
            SrcColor = GL_SRC_COLOR,
            OneMinusSrcColor = GL_ONE_MINUS_SRC_COLOR,
            DstColor  = GL_DST_COLOR,
            OneMinusDstColor = GL_ONE_MINUS_DST_COLOR,
            SrcAlpha = GL_SRC_ALPHA,
            OneMinusSrcAlpha = GL_ONE_MINUS_SRC_ALPHA,
            DstAlpha = GL_DST_ALPHA,
            OneMinusDstAlpha = GL_ONE_MINUS_DST_ALPHA,
            ConstantColor = GL_CONSTANT_COLOR,
            OneMinusConstantColor = GL_ONE_MINUS_CONSTANT_COLOR,
            ConstantAlpha = GL_CONSTANT_ALPHA,
            OneMinusConstantAlpha = GL_ONE_MINUS_CONSTANT_ALPHA
        };

        enum class BlendEquation {
            Add = GL_FUNC_ADD,
            Subtract = GL_FUNC_SUBTRACT,
            ReverseSubtract = GL_FUNC_REVERSE_SUBTRACT,
            Min = GL_MIN,
            Max = GL_MAX
        };

    public:
        RenderState();

        RenderState& depth(const DepthFunction function, const bool write) {
            mDepthTest = true;
            mDepthWrite = write;
            mDepthFunction = function;
            
            return *this;
        }

        RenderState& depth(const bool b) {
            NSM_ASSERT(!b, "True value passed to depth disabler. Use the other overload!");
        
            mDepthTest = false;

            return *this;
        }

        RenderState& cull(const CullFace face, const CullDirection direction) {
            mCullEnabled = true;
            mCullFace = face;
            mCullDirection = direction;

            return *this;
        }

        RenderState& cull(const bool b) {
            NSM_ASSERT(!b, "True value passed to cull disabler. Use the other overload!");
        
            mCullEnabled = false;

            return *this;
        }

        RenderState& blend(const BlendFactor srcRGBA, const BlendFactor dstRGBA, const BlendEquation equation) {
            mBlendEnabled = true;
            mSrcRGB = srcRGBA;
            mDstRGB = dstRGBA;
            mSrcA = srcRGBA;
            mDstA = dstRGBA;
            mBlendEquation = equation;

            return *this;
        }

        RenderState& blend(const BlendFactor srcRGB, const BlendFactor dstRGB, const BlendFactor srcA, const BlendFactor dstA, const BlendEquation equation) {
            mBlendEnabled = true;
            mSrcRGB = srcRGB;
            mDstRGB = dstRGB;
            mSrcA = srcA;
            mDstA = dstA;
            mBlendEquation = equation;

            return *this;
        }

        RenderState& blend(const bool b) {
            NSM_ASSERT(!b, "True value passed to blend disabler. Use the other overload!");
        
            mBlendEnabled = false;

            return *this;
        }

        void apply() const;

    private:
        bool mDepthTest;
        bool mDepthWrite;
        bool mCullEnabled;
        bool mBlendEnabled;

        DepthFunction mDepthFunction;

        CullFace mCullFace;
        CullDirection mCullDirection;

        BlendFactor mSrcRGB, mSrcA;
        BlendFactor mDstRGB, mDstA;
        BlendEquation mBlendEquation;
    };

}
