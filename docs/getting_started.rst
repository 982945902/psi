Getting started
===============

Welcome to SecretFlow PSI Library. There are multiple methods to use PSI/PIR.

* C++ binaries, you could build the binary or with release docker image.
* Python packages

    * `SPU <https://pypi.org/project/spu/>`_ warps the library as Python bindings. You could call PSI/PIR with spu.
    * `SecretFlow <https://pypi.org/project/secretflow/>`_ warps spu further.


* Applications

    * `SCQL <https://www.secretflow.org.cn/docs/scql/latest/zh-Hans>`_ integrates this library to do JOIN operations.
    * `SecretPad <https://www.secretflow.org.cn/docs/quickstart/mvp-platform>`_ provides PSI component.
    * `Easy PSI <https://www.secretflow.org.cn/docs/quickstart/easy-psi>`_ provides most functionality of this library to users.


For PSI, we have a developing v2 PSI.

+------------------------+------------------------------------------------+---------------------------------------------+
|                        | PSI v1 APIs                                    | PSI v2 APIs                                 |
+========================+================================================+=============================================+
| Supported Protocols    | ECDH, BC22, KKRT, ECDH_OPRF_UB, DP_PSI, RR22   | ECDH, KKRT, RR22                            |
+------------------------+------------------------------------------------+---------------------------------------------+
| CSV parser             | Support a subset of csv files.                 | Apache Arrow, support all legal csv files.  |
+------------------------+------------------------------------------------+---------------------------------------------+
| Recovery after failure | Unsupported                                    | Supported                                   |
+------------------------+------------------------------------------------+---------------------------------------------+
| Inner join             | Unsupported at library level                   | Supported natively                          |
+------------------------+------------------------------------------------+---------------------------------------------+
| Release Docker         | Not provided                                   | Provided                                    |
+------------------------+------------------------------------------------+---------------------------------------------+
| Python Binding         | with SPU                                       | Not provided                                |
+------------------------+------------------------------------------------+---------------------------------------------+


Installation
------------

C++ binaries
^^^^^^^^^^^^

Release Docker
""""""""""""""

Please check official release docker image at `dockerhub <https://hub.docker.com/r/secretflow/psi-anolis8>`_. We also have mirrors at Alibaba Cloud: secretflow-registry.cn-hangzhou.cr.aliyuncs.com/secretflow/psi-anolis8.


Building from Source
""""""""""""""""""""

Please see :ref:`building`.


Python packages
^^^^^^^^^^^^^^^

SPU
"""

Please check `SPU Installation Guidelines <https://www.secretflow.org.cn/docs/spu/latest/en-US/getting_started/install>`_.

SecretFlow
""""""""""

Please check `SecretFlow Installation page <https://www.secretflow.org.cn/docs/secretflow/latest/en-US/getting_started/installation>`_.

Applications
^^^^^^^^^^^^

SCQL
""""

Please check `SCQL Quickstart tutorial <https://www.secretflow.org.cn/docs/scql/latest/en-US/intro/tutorial>`_.

SecretPad
"""""""""

Please check `SecretPad handbook <https://www.secretflow.org.cn/docs/quickstart/mvp-platform>`_.

Easy PSI
""""""""

Please check `Easy PSI handbook <https://www.secretflow.org.cn/docs/quickstart/easy-psi>`_.


.. _building:

Building
--------

System Setup
^^^^^^^^^^^^

Dev Docker
""""""""""

We use the same dev docker from secretflow/ubuntu-base-ci::

    ## start container
    docker run -d -it --name psi-dev-$(whoami) \
         --mount type=bind,source="$(pwd)",target=/home/admin/dev/ \
         -w /home/admin/dev \
         --cap-add=SYS_PTRACE --security-opt seccomp=unconfined \
         --cap-add=NET_ADMIN \
         --privileged=true \
         --entrypoint="bash" \
         secretflow/ubuntu-base-ci:latest


    # attach to build container
    docker exec -it psi-dev-$(whoami) bash

Linux
""""""

You need to install:

* gcc>=11.2
* cmake>=3.26
* ninja
* nasm>=2.15
* python>=3.8
* bazel==6.2.1
* golang
* xxd
* lld

Build & UnitTest
^^^^^^^^^^^^^^^^

We use bazel for building and testing::

    # build as debug
    bazel build //... -c dbg

    # build as release
    bazel build //... -c opt

    # test
    bazel test //...



Reporting an Issue
------------------

Please create an issue at `Github Issues <https://github.com/secretflow/psi/issues>`_.

We will look into issues and get back to you soon.

Frequently Asked Questions (FAQ)
--------------------------------

We will collect some popular questions from users and update this part promptly.
