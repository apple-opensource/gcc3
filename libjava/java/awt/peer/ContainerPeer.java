/* Copyright (C) 2000  Free Software Foundation

   This file is part of libgcj.

This software is copyrighted work licensed under the terms of the
Libgcj License.  Please consult the file "LIBGCJ_LICENSE" for
details.  */

package java.awt.peer;

import java.awt.Insets;

public interface ContainerPeer extends ComponentPeer
{
  Insets getInsets();
  void beginValidate();
  void endValidate();
}
