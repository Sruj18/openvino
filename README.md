<div align="center">

<img src="docs/img/openvino-logo-purple-black.png" width="400px">

[![Stable release](https://img.shields.io/badge/version-2022.1-green.svg)](https://github.com/openvinotoolkit/openvino/releases/tag/2022.1)
[![Apache License Version 2.0](https://img.shields.io/badge/license-Apache_2.0-green.svg)](LICENSE)
![GitHub branch checks state](https://img.shields.io/github/checks-status/openvinotoolkit/openvino/master?label=GitHub%20checks)
![Azure DevOps builds (branch)](https://img.shields.io/azure-devops/build/openvinoci/b2bab62f-ab2f-4871-a538-86ea1be7d20f/13?label=Public%20CI)
[![PyPI Status](https://badge.fury.io/py/openvino.svg)](https://badge.fury.io/py/openvino)
[![PyPI Downloads](https://pepy.tech/badge/openvino)](https://pepy.tech/project/openvino)
 
 </div>

## Contents:

 - [What is OpenVINO?](#what-is-openvino-toolkit)
    - [Components](#components)
 - [Supported Hardware matrix](#supported-hardware-matrix)
 - [License](#license)
 - [Documentation](#documentation)
 - [Tutorials](#tutorials)
 - [Products which use OpenVINO](#products-which-use-openvino)
 - [System requirements](#system-requirements)
 - [How to build](#how-to-build)
 - [How to contribute](#how-to-contribute)
 - [Get a support](#get-a-support)
 - [See also](#see-also)

## What is OpenVINO toolkit?

OpenVINO™ is an open-source toolkit for optimizing and deploying AI inference.
 - Boost deep learning performance in computer vision, automatic speech recognition, natural language processing and other common tasks
 - Use models trained with popular frameworks like TensorFlow, PyTorch and more
 - Reduce resource demands and efficiently deploy on a range of Intel® platforms from edge to cloud


This open-source version includes several components: namely [Model Optimizer], [OpenVINO™ Runtime], [Post-Training Optimization Tool], as well as CPU, GPU, MYRIAD, multi device and heterogeneous plugins to accelerate deep learning inferencing on Intel® CPUs and Intel® Processor Graphics.
It supports pre-trained models from the [Open Model Zoo], along with 100+ open
source and public models in popular formats such as TensorFlow, ONNX, PaddlePaddle, MXNet, Caffe, Kaldi.

### Components
* [OpenVINO™ Runtime] - is a set of C++ libraries with C and Python bindings providing a common API to deliver inference solutions on the platform of your choice.
    * [core](https://github.com/openvinotoolkit/openvino/tree/master/src/core) - provides the base API for model representation and modification.
    * [inference](https://github.com/openvinotoolkit/openvino/tree/master/src/inference) - provides an API to infer models on device.
    * [transformations](https://github.com/openvinotoolkit/openvino/tree/master/src/common/transformations) - contains the set of common transformations which are used in OpenVINO plugins.
    * [low precision transformations](https://github.com/openvinotoolkit/openvino/tree/master/src/common/low_precision_transformations) - contains the set of transformations which are used in low precision models
    * [bindings](https://github.com/openvinotoolkit/openvino/tree/master/src/bindings) - contains all available OpenVINO bindings which are maintained by OpenVINO team.
        * [c](https://github.com/openvinotoolkit/openvino/tree/master/src/bindings/c) - provides C API for OpenVINO™ Runtime
        * [python](https://github.com/openvinotoolkit/openvino/tree/master/src/bindings/python) - Python API for OpenVINO™ Runtime
* [Plugins](https://github.com/openvinotoolkit/openvino/tree/master/src/plugins) - contains OpenVINO plugins which are maintained in open-source by OpenVINO team. For more information please take a look to the [list of supported devices](#supported-hardware-matrix).
* [Frontends](https://github.com/openvinotoolkit/openvino/tree/master/src/frontends) - contains available OpenVINO frontends which allow to read model from native framework format.
* [Model Optimizer] - is a cross-platform command-line tool that facilitates the transition between training and deployment environments, performs static model analysis, and adjusts deep learning models for optimal execution on end-point target devices.
* [Post-Training Optimization Tool] - is designed to accelerate the inference of deep learning models by applying special methods without model retraining or fine-tuning, for example, post-training 8-bit quantization. 
* [Samples] - applications on C, C++ and Python languages which shows basic use cases of OpenVINO usages.

## Supported Hardware matrix

The OpenVINO™ Runtime can infer models on different hardware devices. This section provides the list of supported devices.

<table>
    <thead>
        <tr>
            <th>Device</th>
            <th>Plugin</th>
            <th>Library</th>
            <th>ShortDescription</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td rowspan=2>CPU</td>
            <td> <a href="https://docs.openvino.ai/nightly/openvino_docs_OV_UG_supported_plugins_CPU.html#doxid-openvino-docs-o-v-u-g-supported-plugins-c-p-u">Intel CPU</a></tb>
            <td><b><i><a href="https://github.com/openvinotoolkit/openvino/tree/master/src/plugins/intel_cpu">openvino_intel_cpu_plugin</a></i></b></td>
            <td>Intel Xeon with Intel® Advanced Vector Extensions 2 (Intel® AVX2), Intel® Advanced Vector Extensions 512 (Intel® AVX-512), and AVX512_BF16, Intel Core Processors with Intel AVX2, Intel Atom Processors with Intel® Streaming SIMD Extensions (Intel® SSE)</td>
        </tr>
        <tr>
            <td> <a href="https://docs.openvino.ai/nightly/openvino_docs_OV_UG_supported_plugins_ARM_CPU.html">ARM CPU</a></tb>
            <td><b><i><a href="https://github.com/openvinotoolkit/openvino_contrib/tree/master/modules/arm_plugin">openvino_arm_cpu_plugin</a></i></b></td>
            <td>Raspberry Pi™ 4 Model B, Apple® Mac mini with M1 chip, NVIDIA® Jetson Nano™, Android™ devices
        </tr>
        <tr>
            <td>GPU</td>
            <td><a href="https://docs.openvino.ai/nightly/openvino_docs_OV_UG_supported_plugins_GPU.html#doxid-openvino-docs-o-v-u-g-supported-plugins-g-p-u">Intel GPU</a></td>
            <td><b><i><a href="https://github.com/openvinotoolkit/openvino/tree/master/src/plugins/intel_gpu">openvino_intel_gpu_plugin</a></i></b></td>
            <td>Intel Processor Graphics, including Intel HD Graphics and Intel Iris Graphics</td>
        </tr>
        <tr>
            <td>GNA</td>
            <td><a href="https://docs.openvino.ai/nightly/openvino_docs_OV_UG_supported_plugins_GNA.html#doxid-openvino-docs-o-v-u-g-supported-plugins-g-n-a">Intel GNA</a></td>
            <td><b><i><a href="https://github.com/openvinotoolkit/openvino/tree/master/src/plugins/intel_gna">openvino_intel_gna_plugin</a></i></b></td>
            <td>Intel Speech Enabling Developer Kit, Amazon Alexa* Premium Far-Field Developer Kit, Intel Pentium Silver J5005 Processor, Intel Pentium Silver N5000 Processor, Intel Celeron J4005 Processor, Intel Celeron J4105 Processor, Intel Celeron Processor N4100, Intel Celeron Processor N4000, Intel Core i3-8121U Processor, Intel Core i7-1065G7 Processor, Intel Core i7-1060G7 Processor, Intel Core i5-1035G4 Processor, Intel Core i5-1035G7 Processor, Intel Core i5-1035G1 Processor, Intel Core i5-1030G7 Processor, Intel Core i5-1030G4 Processor, Intel Core i3-1005G1 Processor, Intel Core i3-1000G1 Processor, Intel Core i3-1000G4 Processor</td>
        </tr>
        <tr>
            <td>VPU</td>
            <td><a href="https://docs.openvino.ai/nightly/openvino_docs_IE_DG_supported_plugins_VPU.html#doxid-openvino-docs-i-e-d-g-supported-plugins-v-p-u">Myriad plugin</a></td>
            <td><b><i><a href="https://github.com/openvinotoolkit/openvino/tree/master/src/plugins/intel_myriad">openvino_intel_myriad_plugin</a></i></b></td>
            <td>Intel® Neural Compute Stick 2 powered by the Intel® Movidius™ Myriad™ X</td>
        </tr>
    </tbody>
</table>

Also OpenVINO™ Toolkit contains several plugins which should simplify to load model on several hardware devices:
<table>
    <thead>
        <tr>
            <th>Plugin</th>
            <th>Library</th>
            <th>ShortDescription</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><a href="https://docs.openvino.ai/nightly/openvino_docs_IE_DG_supported_plugins_AUTO.html#doxid-openvino-docs-i-e-d-g-supported-plugins-a-u-t-o">Auto</a></td>
            <td><b><i><a href="https://github.com/openvinotoolkit/openvino/tree/master/src/plugins/auto">openvino_auto_plugin</a></i></b></td>
            <td>Auto plugin enables selecting Intel device for inference automatically</td>
        </tr>
        <tr>
            <td><a href="https://docs.openvino.ai/nightly/openvino_docs_OV_UG_Automatic_Batching.html">Auto Batch</a></td>
            <td><b><i><a href="https://github.com/openvinotoolkit/openvino/tree/master/src/plugins/auto_batch">openvino_auto_batch_plugin</a></i></b></td>
            <td>Auto batch plugin performs on-the-fly automatic batching (i.e. grouping inference requests together) to improve device utilization, with no programming effort from the user</td>
        </tr>
        <tr>
            <td><a href="https://docs.openvino.ai/nightly/openvino_docs_OV_UG_Hetero_execution.html#doxid-openvino-docs-o-v-u-g-hetero-execution">Hetero</a></td>
            <td><b><i><a href="https://github.com/openvinotoolkit/openvino/tree/master/src/plugins/hetero">openvino_hetero_plugin</a></i></b></td>
            <td>Heterogeneous execution enables automatic inference splitting between several devices</td>
        </tr>
        <tr>
            <td><a href="https://docs.openvino.ai/nightly/openvino_docs_OV_UG_Running_on_multiple_devices.html#doxid-openvino-docs-o-v-u-g-running-on-multiple-devices">Multi</a></td>
            <td><b><i><a href="https://github.com/openvinotoolkit/openvino/tree/master/src/plugins/auto">openvino_auto_plugin</a></i></b></td>
            <td>Multi plugin enables simultaneous inference of the same model on several devices in parallel</td>
        </tr>
    </tbody>
</table>

## License
OpenVINO™ Toolkit is licensed under [Apache License Version 2.0](LICENSE).
By contributing to the project, you agree to the license and copyright terms therein and release your contribution under these terms.

## Documentation

### User documentation

The latest documentation for OpenVINO™ Toolkit is available [here](https://docs.openvino.ai/). This documentation contains detailed information about all OpenVINO components and provides all important information which could be needed if you create an application which is based on binary OpenVINO distribution or own OpenVINO version without source code modification.

### Developer documentation

[Developer documentation](#todo-add) contains information about architectural decisions which are applied inside the OpenVINO components. This documentation has all necessary information which could be needed in order to contribute to OpenVINO.

## Tutorials

The list of OpenVINO tutorials:

- [Jupiter notebooks](https://github.com/openvinotoolkit/openvino_notebooks)

## Products which use OpenVINO

- [OpenCV](https://opencv.org/)
- [ONNX Runtime](https://onnxruntime.ai/)
- [OpenVINO™ Integration with TensorFlow](https://www.intel.com/content/www/us/en/developer/tools/devcloud/edge/build/ovtfoverview.html)
- [TNN](https://github.com/Tencent/TNN/tree/master)

## System requirements

The full information about system requirements depends on platform and available in section `System requirement` on dedicated pages:
- [Linux](https://docs.openvino.ai/latest/openvino_docs_install_guides_installing_openvino_linux.html)
- [Windows](https://docs.openvino.ai/latest/openvino_docs_install_guides_installing_openvino_windows.html)
- [macOS](https://docs.openvino.ai/latest/openvino_docs_install_guides_installing_openvino_macos.html)
- [Raspbian](https://docs.openvino.ai/latest/openvino_docs_install_guides_installing_openvino_raspbian.html)

## How to build

Please take a look to [OpenVINO Wiki](https://github.com/openvinotoolkit/openvino/wiki#how-to-build) to get more information about OpenVINO build process.

## How to contribute

See [CONTRIBUTING](./CONTRIBUTING.md) for details. Thank you!

## Get a support

Please report questions, issues and suggestions using:

* [GitHub* Issues](https://github.com/openvinotoolkit/openvino/issues)
* The [`openvino`](https://stackoverflow.com/questions/tagged/openvino) tag on StackOverflow\*
* [Forum](https://software.intel.com/en-us/forums/computer-vision)

## See also

* [OpenVINO Wiki](https://github.com/openvinotoolkit/openvino/wiki)
* [OpenVINO Storage](https://storage.openvinotoolkit.org/)
* Additional OpenVINO™ toolkit modules: 
    * [openvino_contrib](https://github.com/openvinotoolkit/openvino_contrib)
* [Intel® Distribution of OpenVINO™ toolkit Product Page](https://software.intel.com/content/www/us/en/develop/tools/openvino-toolkit.html)
* [Intel® Distribution of OpenVINO™ toolkit Release Notes](https://software.intel.com/en-us/articles/OpenVINO-RelNotes)
* [Neural Network Compression Framework (NNCF)](https://github.com/openvinotoolkit/nncf) - a suite of advanced algorithms for model inference optimization including quantization, filter pruning, binarization and sparsity
* [OpenVINO™ Training Extensions (OTE)](https://github.com/openvinotoolkit/training_extensions) - convenient environment to train Deep Learning models and convert them using OpenVINO for optimized inference.
* [OpenVINO™ Model Server (OVMS)](https://github.com/openvinotoolkit/model_server) - a scalable, high-performance solution for serving deep learning models optimized for Intel architectures
* [DL Workbench](https://docs.openvino.ai/nightly/workbench_docs_Workbench_DG_Introduction.html) - An alternative, web-based version of OpenVINO designed to make production of pretrained deep learning models significantly easier.
* [Computer Vision Annotation Tool (CVAT)](https://github.com/openvinotoolkit/cvat) - an online, interactive video and image annotation tool for computer vision purposes.
* [Dataset Management Framework (Datumaro)](https://github.com/openvinotoolkit/datumaro) - a framework and CLI tool to build, transform, and analyze datasets.

---
\* Other names and brands may be claimed as the property of others.

[Open Model Zoo]:https://github.com/openvinotoolkit/open_model_zoo
[OpenVINO™ Runtime]:https://docs.openvino.ai/latest/openvino_docs_OV_UG_OV_Runtime_User_Guide.html
[Model Optimizer]:https://docs.openvino.ai/latest/openvino_docs_MO_DG_Deep_Learning_Model_Optimizer_DevGuide.html
[Post-Training Optimization Tool]:https://docs.openvino.ai/latest/pot_introduction.html
[Samples]:https://github.com/openvinotoolkit/openvino/tree/master/samples
