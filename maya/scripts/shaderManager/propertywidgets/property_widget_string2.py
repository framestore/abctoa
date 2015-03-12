# Alembic Holder
# Copyright (c) 2014, Gael Honorez, All rights reserved.
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 3.0 of the License, or (at your option) any later version.
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Lesser General Public License for more details.
# You should have received a copy of the GNU Lesser General Public
# License along with this library.

from PySide.QtGui import *
from PySide.QtCore import *
from arnold import *
from property_widget import *
class PropertyWidgetString2(PropertyWidget):
   def __init__(self, controller, default, name, parent = None):
      PropertyWidget.__init__(self, name, parent)

      self.paramName = name
      self.controller = controller
      self.controller.setPropertyValue.connect(self.changed)
      self.controller.reset.connect(self.resetValue)
      self.widget = QLineEdit(self)
      self.default = default



      self.widget.returnPressed.connect(self.TextChanged)
      self.layout().addWidget(self.widget)
   
   def TextChanged(self):
      value = self.widget.text()
      self.controller.mainEditor.propertyChanged(dict(propname=self.paramName, default=value == self.default, value=value))


   def changed(self, message):

      value = message["value"]
      self.widget.setText(value)


   def resetValue(self):
      self.widget.setText(self.default)