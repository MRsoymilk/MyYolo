## Linux

### LabelImg

1. Create venv dir

```bash
python -m venv venv
source ./venv/bin/active
```

2. install lib

```bash
pip install pyqt5
pip install lxml
```

3. build

```bash
make qt5py3
```

4. run

```bash
python labelImg.py
```


### Problem

1. 
```bash
Traceback (most recent call last):
  File "/home/vv/project/my-yolo/project/tools/linux/labelImg-1.8.1/libs/canvas.py", line 458, in paintEvent
    p.drawLine(self.prevPoint.x(), 0, self.prevPoint.x(), self.pixmap.height())
TypeError: arguments did not match any overloaded call:
  drawLine(self, l: QLineF): argument 1 has unexpected type 'float'
  drawLine(self, line: QLine): argument 1 has unexpected type 'float'
  drawLine(self, x1: int, y1: int, x2: int, y2: int): argument 1 has unexpected type 'float'
  drawLine(self, p1: QPoint, p2: QPoint): argument 1 has unexpected type 'float'
  drawLine(self, p1: Union[QPointF, QPoint], p2: Union[QPointF, QPoint]): argument 1 has unexpected type 'float'
Aborted
```

2. 
```bash
Cancel creation.
Image:/home/vv/tmp/my_adventure/1.jpg -> Annotation:/home/vv/tmp/my_adventure/1.xml
Traceback (most recent call last):
  File "/home/vv/project/my-yolo/project/tools/linux/labelImg-1.8.1/libs/canvas.py", line 438, in paintEvent
    shape.paint(p)
  File "/home/vv/project/my-yolo/project/tools/linux/labelImg-1.8.1/libs/shape.py", line 130, in paint
    painter.drawText(min_x, min_y, self.label)
TypeError: arguments did not match any overloaded call:
  drawText(self, p: Union[QPointF, QPoint], s: Optional[str]): argument 1 has unexpected type 'float'
  drawText(self, rectangle: QRectF, flags: int, text: Optional[str]): argument 1 has unexpected type 'float'
  drawText(self, rectangle: QRect, flags: int, text: Optional[str]): argument 1 has unexpected type 'float'
  drawText(self, rectangle: QRectF, text: Optional[str], option: QTextOption = QTextOption()): argument 1 has unexpected type 'float'
  drawText(self, p: QPoint, s: Optional[str]): argument 1 has unexpected type 'float'
  drawText(self, x: int, y: int, width: int, height: int, flags: int, text: Optional[str]): argument 1 has unexpected type 'float'
  drawText(self, x: int, y: int, s: Optional[str]): argument 1 has unexpected type 'float'
Aborted
```

3.
```bash
Cancel creation.
Image:/home/vv/tmp/my_adventure/1.jpg -> Annotation:/home/vv/tmp/my_adventure/1.xml
Traceback (most recent call last):
  File "/home/vv/project/my-yolo/project/tools/linux/labelImg-1.8.1/libs/canvas.py", line 438, in paintEvent
    shape.paint(p)
  File "/home/vv/project/my-yolo/project/tools/linux/labelImg-1.8.1/libs/shape.py", line 130, in paint
    painter.drawText(min_x, min_y, self.label)
TypeError: arguments did not match any overloaded call:
  drawText(self, p: Union[QPointF, QPoint], s: Optional[str]): argument 1 has unexpected type 'float'
  drawText(self, rectangle: QRectF, flags: int, text: Optional[str]): argument 1 has unexpected type 'float'
  drawText(self, rectangle: QRect, flags: int, text: Optional[str]): argument 1 has unexpected type 'float'
  drawText(self, rectangle: QRectF, text: Optional[str], option: QTextOption = QTextOption()): argument 1 has unexpected type 'float'
  drawText(self, p: QPoint, s: Optional[str]): argument 1 has unexpected type 'float'
  drawText(self, x: int, y: int, width: int, height: int, flags: int, text: Optional[str]): argument 1 has unexpected type 'float'
  drawText(self, x: int, y: int, s: Optional[str]): argument 1 has unexpected type 'float'
Aborted
```

### Deal

labelImg.py
```python
-        bar.setValue(bar.value() + bar.singleStep() * units)
+        bar.setValue(int(bar.value() + bar.singleStep() * units))
```

canvas.py
```python
-            p.drawRect(leftTop.x(), leftTop.y(), rectWidth, rectHeight)
+            p.drawRect(int(leftTop.x()), int(leftTop.y()), int(rectWidth), int(rectHeight))


-            p.drawLine(self.prevPoint.x(), 0, self.prevPoint.x(), self.pixmap.height())
-            p.drawLine(0, self.prevPoint.y(), self.pixmap.width(), self.prevPoint.y())
+            p.drawLine(int(self.prevPoint.x()), 0, int(self.prevPoint.x()), int(self.pixmap.height()))
+            p.drawLine(0, int(self.prevPoint.y()), int(self.pixmap.width()), int(self.prevPoint.y()))
```

shape.py
```python
-                    painter.drawText(min_x, min_y, self.label)
+                    painter.drawText(int(min_x), int(min_y), self.label)
```
