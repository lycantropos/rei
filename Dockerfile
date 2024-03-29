ARG IMAGE_NAME
ARG IMAGE_VERSION

FROM ${IMAGE_NAME}:${IMAGE_VERSION}

RUN pip install --upgrade pip setuptools

WORKDIR /opt/rei

COPY requirements-setup.txt .
COPY requirements-tests.txt .
RUN pip install -r requirements-tests.txt
COPY requirements.txt .

COPY README.md .
COPY pytest.ini .
COPY setup.py .
COPY rei rei/
COPY src/ src/
COPY tests/ tests/

RUN pip install -e .
