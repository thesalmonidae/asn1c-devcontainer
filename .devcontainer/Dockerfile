ARG     ROCKYLINUX_TAG
FROM    rockylinux:${ROCKYLINUX_TAG}

ARG     DEV_USER
ARG     ASN1C_VERSION
ARG     CJSON_VERSION

RUN     microdnf install -y \
                autoconf \
                automake \
                cmake \
                curl \
                gcc \
                git \
                gzip \
                libtool \
                make \
                tar

RUN     useradd -m ${DEV_USER} &&\
        curl -LO https://github.com/vlm/asn1c/archive/refs/tags/v${ASN1C_VERSION}.tar.gz &&\
        tar -zxf v${ASN1C_VERSION}.tar.gz &&\
        cd asn1c-${ASN1C_VERSION} &&\
        test -f configure || autoreconf -iv &&\
        ./configure &&\
        make &&\
        make install && \
        cd .. &&\
        rm -rf asn1c-${ASN1C_VERSION} &&\
        rm v${ASN1C_VERSION}.tar.gz &&\
        curl -LO https://github.com/DaveGamble/cJSON/archive/refs/tags/v${CJSON_VERSION}.tar.gz &&\
        tar -zxf v${CJSON_VERSION}.tar.gz &&\
        cd cJSON-${CJSON_VERSION} &&\
        mkdir build &&\
        cd build &&\
        cmake .. &&\
        make &&\
        make install &&\
        cd ../.. &&\
        rm -rf cJSON-${CJSON_VERSION} &&\
        rm v${CJSON_VERSION}.tar.gz

USER    ${DEV_USER}
