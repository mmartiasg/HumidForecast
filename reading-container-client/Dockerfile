FROM python
WORKDIR /app
COPY requirements.txt requirements.txt
COPY read.py read.py
RUN pip install -r requirements.txt
CMD ["python", "read.py"]