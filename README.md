# smath - A Simple C++ Math Library

> ## Vector (Up to 16)
> Supported Operations
> - Length / Length2
> - Normalize / Normalize or Zero
> - Inner product / Outer product
> - Cross product
> - Element wise operations
> - Projection on Vector
> - Rotate on arbitrary axis
> - Expand/Combine with other vector
> - Swizzling access via [] operator

> ## Matrix (Up to 16x16)
> Supported Operations
> - Translation/Rotation
> - Sub-matrix extraction
> - Determinant
> - Transpose
> - Invert/Adjoint Matrix
> - Matrices Multiplication
> - Projection Matrices
> - View Matrices
> - Rotation on arbitrary axis

> ## Quaternion
> Supported Operations
> - Normalize / Normalize or Zero
> - from_mat3() / to_mat3()
> - from_mat4() / to_mat4()
> - conjugate

For coordinate system relevant computation (projection matrix), they are all based on `right-handed y-up` system, assuming camera is looking at the direction -z.


