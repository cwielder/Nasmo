#pragma once

#include <nsm/gfx/OpenGL.h>
#include <nsm/debug/Assert.h>

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

        enum class StencilFunction {
            Never = GL_NEVER,
            Less = GL_LESS,
            Equal = GL_EQUAL,
            LessEqual = GL_LEQUAL,
            Greater = GL_GREATER,
            NotEqual = GL_NOTEQUAL,
            GreaterEqual = GL_GEQUAL,
            Always = GL_ALWAYS
        };

        enum class StencilOperation {
            Keep = GL_KEEP,
            Zero = GL_ZERO,
            Replace = GL_REPLACE,
            Increment = GL_INCR,
            IncrementWrap = GL_INCR_WRAP,
            Decrement = GL_DECR,
            DecrementWrap = GL_DECR_WRAP,
            Invert = GL_INVERT
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

        enum class StateBit : u8 {
            Depth = 1 << 0,
            Stencil = 1 << 1,
            Cull = 1 << 2,
            Blend = 1 << 3,

            All = Depth | Stencil | Cull | Blend
        };

        friend constexpr StateBit operator|(const StateBit a, const StateBit b) {
            return static_cast<StateBit>(static_cast<u8>(a) | static_cast<u8>(b));
        }

        friend constexpr u8 operator&(const StateBit a, const StateBit b) {
            return static_cast<u8>(a) & static_cast<u8>(b);
        }

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

        RenderState& stencil(const StencilFunction function, const StencilOperation fail, const StencilOperation depthFail, const StencilOperation depthPass, const u8 mask, const u8 reference) {
            mStencilTest = true;
            mStencilFunction = function;
            mStencilFail = fail;
            mDepthFail = depthFail;
            mDepthPass = depthPass;
            mStencilMask = mask;
            mStencilReference = reference;

            return *this;
        }

        RenderState& stencil(const bool b) {
            NSM_ASSERT(!b, "True value passed to stencil disabler. Use the other overload!");
        
            mStencilTest = false;

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

        void apply(const StateBit states = StateBit::All) const;

    private:
        bool mDepthTest;
        bool mDepthWrite;
        bool mCullEnabled;
        bool mBlendEnabled;
        bool mStencilTest;

        u8 mStencilMask;
        u8 mStencilReference;
        StencilFunction mStencilFunction;
        StencilOperation mStencilFail, mDepthFail, mDepthPass;

        DepthFunction mDepthFunction;

        CullFace mCullFace;
        CullDirection mCullDirection;

        BlendFactor mSrcRGB, mSrcA;
        BlendFactor mDstRGB, mDstA;
        BlendEquation mBlendEquation;
    };

}
