#ifndef _PRELU_PLUGIN_H
#define _PRELU_PLUGIN_H

#include "API.h"
#include <string>
#include <vector>
#include "NvInfer.h"

#if NV_TENSORRT_MAJOR >= 8
#define TRT_NOEXCEPT noexcept
#define TRT_CONST_ENQUEUE const
#else
#define TRT_NOEXCEPT
#define TRT_CONST_ENQUEUE
#endif

namespace nvinfer1
{
    class PReluPlugin: public IPluginV2IOExt
    {
        public:
            PReluPlugin(const std::vector<float>& gamma);
            PReluPlugin(const void* data, size_t length);

            ~PReluPlugin();

            int getNbOutputs() const TRT_NOEXCEPT override
            {
                return 1;
            }

            Dims getOutputDimensions(int index, const Dims* inputs, int nbInputDims) TRT_NOEXCEPT override;

            int initialize() TRT_NOEXCEPT override;

            virtual void terminate() TRT_NOEXCEPT override {};

            virtual size_t getWorkspaceSize(int maxBatchSize) const TRT_NOEXCEPT override { return 0;}

            virtual int enqueue(int batchSize, const void*const * inputs, void*TRT_CONST_ENQUEUE* outputs, void* workspace, cudaStream_t stream) TRT_NOEXCEPT override;

            virtual size_t getSerializationSize() const TRT_NOEXCEPT override;

            virtual void serialize(void* buffer) const TRT_NOEXCEPT override;

            bool supportsFormatCombination(int pos, const PluginTensorDesc* inOut, int nbInputs, int nbOutputs) const TRT_NOEXCEPT override {
                return inOut[pos].format == TensorFormat::kLINEAR && inOut[pos].type == DataType::kFLOAT;
            }

            const char* getPluginType() const TRT_NOEXCEPT override;

            const char* getPluginVersion() const TRT_NOEXCEPT override;

            void destroy() TRT_NOEXCEPT override;

            IPluginV2IOExt* clone() const TRT_NOEXCEPT override;

            void setPluginNamespace(const char* pluginNamespace) TRT_NOEXCEPT override;

            const char* getPluginNamespace() const TRT_NOEXCEPT override;

            DataType getOutputDataType(int index, const nvinfer1::DataType* inputTypes, int nbInputs) const TRT_NOEXCEPT override;

            bool isOutputBroadcastAcrossBatch(int outputIndex, const bool* inputIsBroadcasted, int nbInputs) const TRT_NOEXCEPT override;

            bool canBroadcastInputAcrossBatch(int inputIndex) const TRT_NOEXCEPT override;

            void attachToContext(
                    cudnnContext* cudnnContext, cublasContext* cublasContext, IGpuAllocator* gpuAllocator) TRT_NOEXCEPT override;

            void configurePlugin(const PluginTensorDesc* in, int nbInput, const PluginTensorDesc* out, int nbOutput) TRT_NOEXCEPT override;

            void detachFromContext() TRT_NOEXCEPT override;

            int input_size_;
        private:
            void forwardGpu(const float *const * inputs, float* output, cudaStream_t stream, int batchSize = 1);
            int thread_count_ = 256;
            std::vector<float> gamma_;
            const char* mPluginNamespace;
    };

    class PReluPluginCreator : public IPluginCreator
    {
        public:
            PReluPluginCreator();

            ~PReluPluginCreator() TRT_NOEXCEPT override = default;

            const char* getPluginName() const TRT_NOEXCEPT override;

            const char* getPluginVersion() const TRT_NOEXCEPT override;

            const PluginFieldCollection* getFieldNames() TRT_NOEXCEPT override;

            IPluginV2IOExt* createPlugin(const char* name, const PluginFieldCollection* fc) TRT_NOEXCEPT override;

            IPluginV2IOExt* deserializePlugin(const char* name, const void* serialData, size_t serialLength) TRT_NOEXCEPT override;

            void setPluginNamespace(const char* libNamespace) TRT_NOEXCEPT override
            {
                mNamespace = libNamespace;
            }

            const char* getPluginNamespace() const TRT_NOEXCEPT override
            {
                return mNamespace.c_str();
            }

        private:
            std::string mNamespace;
            static PluginFieldCollection mFC;
            static std::vector<PluginField> mPluginAttributes;
    };
};
#endif 
