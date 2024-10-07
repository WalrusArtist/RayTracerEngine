/*
    This class `GTform` (Geometric Transformation) defines a 4x4 transformation matrix used to represent and apply transformations (translation, rotation, and scaling) in 3D space. It supports forward and inverse transformations, and is integral to transforming objects, vectors, and rays in a ray tracing engine. Here's a breakdown of the key functionalities:

    1. **Constructor and Destructor**:
       - The default constructor initializes the forward (`m_fwdtfm`) and backward (`m_bcktfm`) transformation matrices to identity matrices.
       - Another constructor allows initialization with custom 4x4 matrices, ensuring that both matrices are valid (4x4) or throws an exception.
       - The destructor performs cleanup, though it has no specific behavior beyond default destruction.

    2. **Setting Transformations (`SetTransform`)**:
       This method applies translation, rotation, and scaling to build the forward transformation matrix:
       - **Translation**: A translation matrix is built by setting the 4th column to the translation vector components.
       - **Rotation**: Three rotation matrices (X, Y, Z) are constructed using standard 3D rotation matrix formulas based on input angles.
       - **Scaling**: A scaling matrix is created with the input scale values along each axis.
       - The final forward transformation matrix (`m_fwdtfm`) is constructed by multiplying these matrices in a specific order: 
         `Translation * Scale * RotationX * RotationY * RotationZ`.
       - The backward transformation matrix (`m_bcktfm`) is calculated as the inverse of the forward matrix, allowing reverse transformations.

    3. **Getters**:
       - `GetForward()`: Returns the forward transformation matrix.
       - `GetBackward()`: Returns the backward transformation matrix.

    4. **Applying Transformations to Rays and Vectors (`Apply`)**:
       - **For Rays**: 
         The method `Apply(const waRT::Ray &inputRay, bool dirFlag)` applies either the forward or backward transformation to the input ray, transforming its two endpoints (`m_point1` and `m_point2`) and updating the ray's direction vector (`m_lab`).
         The direction flag (`dirFlag`) controls whether the forward or backward transformation is used:
         - `true`: Uses the forward transformation.
         - `false`: Uses the backward transformation.
       - **For Vectors**: 
         The method `Apply(const qbVector<double> &inputVector, bool dirFlag)` transforms a 3D vector by converting it to a 4D vector (homogeneous coordinates) and applying the appropriate transformation matrix. The resulting 4D vector is then converted back to a 3D vector.

    5. **Operator Overloading**:
       - **Multiplication (`operator*`)**: Combines two `GTform` objects by multiplying their forward matrices, creating a new `GTform` with the resulting forward matrix and its inverse as the backward matrix. This allows concatenation of transformations.
       - **Assignment (`operator=`)**: Assigns the forward and backward matrices from another `GTform` object to the current instance.

    6. **Printing Utilities (`PrintMatrix`, `PrintVector`)**:
       These functions provide utility methods for printing the contents of the transformation matrices and vectors in a readable format.
       - `PrintMatrix(bool dirFlag)`: Prints either the forward or backward matrix depending on the direction flag.
       - `Print(const qbMatrix2<double> &matrix)`: Prints the elements of a 4x4 matrix.
       - `PrintVector(const qbVector<double> &inputVector)`: Prints the elements of a vector.

    Summary:
    - The `GTform` class encapsulates a 4x4 transformation matrix for performing geometric transformations such as translation, rotation, and scaling.
    - It supports both forward and inverse transformations and can be applied to rays and vectors, making it fundamental for object and ray transformations in the ray tracing engine.
    - The class provides methods for constructing transformation matrices, applying them to objects, and printing their results for debugging.
*/

#include "gtfm.hpp"

waRT::GTform::GTform() {
	m_fwdtfm.SetToIdentity();
	m_bcktfm.SetToIdentity();
}

waRT::GTform::~GTform(){}

waRT::GTform::GTform(const qbMatrix2<double> &fwd, const qbMatrix2<double> &bck) {
	if ((fwd.GetNumRows() != 4) || (fwd.GetNumCols() != 4) ||
		(bck.GetNumRows() != 4) || (bck.GetNumCols() != 4)) {
		throw std::invalid_argument("Cannot construct GTform, inputs are not all 4x4.");
	}
	m_fwdtfm = fwd;
	m_bcktfm = bck;
}

void waRT::GTform::SetTransform(const qbVector<double> &translation,
				                const qbVector<double> &rotation,
				                const qbVector<double> &scale) {
	qbMatrix2<double> translationMatrix	{4, 4};
	qbMatrix2<double> rotationMatrixX	{4, 4};
	qbMatrix2<double>	rotationMatrixY	{4, 4};
	qbMatrix2<double> rotationMatrixZ	{4, 4};
	qbMatrix2<double>	scaleMatrix	{4, 4};

	translationMatrix.SetToIdentity();
	rotationMatrixX.SetToIdentity();
	rotationMatrixY.SetToIdentity();
	rotationMatrixZ.SetToIdentity();
	scaleMatrix.SetToIdentity();

	translationMatrix.SetElement(0, 3, translation.GetElement(0));
	translationMatrix.SetElement(1, 3, translation.GetElement(1));
	translationMatrix.SetElement(2, 3, translation.GetElement(2));

	rotationMatrixZ.SetElement(0, 0, cos(rotation.GetElement(2)));
	rotationMatrixZ.SetElement(0, 1, -sin(rotation.GetElement(2)));
	rotationMatrixZ.SetElement(1, 0, sin(rotation.GetElement(2)));
	rotationMatrixZ.SetElement(1, 1, cos(rotation.GetElement(2)));
	
	rotationMatrixY.SetElement(0, 0, cos(rotation.GetElement(1)));
	rotationMatrixY.SetElement(0, 2, sin(rotation.GetElement(1)));
	rotationMatrixY.SetElement(2, 0, -sin(rotation.GetElement(1)));
	rotationMatrixY.SetElement(2, 2, cos(rotation.GetElement(1)));
	
	rotationMatrixX.SetElement(1, 1, cos(rotation.GetElement(0)));
	rotationMatrixX.SetElement(1, 2, -sin(rotation.GetElement(0)));
	rotationMatrixX.SetElement(2, 1, sin(rotation.GetElement(0)));
	rotationMatrixX.SetElement(2, 2, cos(rotation.GetElement(0)));

	scaleMatrix.SetElement(0, 0, scale.GetElement(0));
	scaleMatrix.SetElement(1, 1, scale.GetElement(1));
	scaleMatrix.SetElement(2, 2, scale.GetElement(2));

	m_fwdtfm = translationMatrix * 
			   scaleMatrix *
			   rotationMatrixX *
			   rotationMatrixY *
			   rotationMatrixZ;

	m_bcktfm = m_fwdtfm;
	m_bcktfm.Inverse();		
	
}

qbMatrix2<double> waRT::GTform::GetForward()  { return m_fwdtfm;}
qbMatrix2<double> waRT::GTform::GetBackward() { return m_bcktfm;}

waRT::Ray waRT::GTform::Apply(const waRT::Ray &inputRay, bool dirFlag) {
	waRT::Ray outputRay;
	
	if (dirFlag) {
		outputRay.m_point1 = this -> Apply(inputRay.m_point1, waRT::FWDTFORM);
		outputRay.m_point2 = this -> Apply(inputRay.m_point2, waRT::FWDTFORM);
		outputRay.m_lab = outputRay.m_point2 - outputRay.m_point1;
	} else {
		outputRay.m_point1 = this -> Apply(inputRay.m_point1, waRT::BCKTFORM);
		outputRay.m_point2 = this -> Apply(inputRay.m_point2, waRT::BCKTFORM);
		outputRay.m_lab = outputRay.m_point2 - outputRay.m_point1;
	}
	return outputRay;
}

qbVector<double> waRT::GTform::Apply(const qbVector<double> &inputVector, bool dirFlag) {
	std::vector<double> tempData {inputVector.GetElement(0),
					              inputVector.GetElement(1),
					              inputVector.GetElement(2),
					              1.0 };
	qbVector<double> tempVector {tempData};
	qbVector<double> resultVector;
	
	if (dirFlag){
		resultVector = m_fwdtfm * tempVector;
	} else {
		resultVector = m_bcktfm * tempVector;
	}

	qbVector<double> outputVector {std::vector<double> { resultVector.GetElement(0),
								                         resultVector.GetElement(1),
								                         resultVector.GetElement(2) }};													
	return outputVector;
}

namespace waRT {
	waRT::GTform operator* (const waRT::GTform &lhs, const waRT::GTform &rhs) {
		qbMatrix2<double> fwdResult = lhs.m_fwdtfm * rhs.m_fwdtfm;
		qbMatrix2<double> bckResult = fwdResult;
		bckResult.Inverse();
		waRT::GTform finalResult (fwdResult, bckResult);
		return finalResult;
	}
}

waRT::GTform waRT::GTform::operator= (const waRT::GTform &rhs) {
	if (this != &rhs) {
		m_fwdtfm = rhs.m_fwdtfm;
		m_bcktfm = rhs.m_bcktfm;
	}
	return *this;
}

void waRT::GTform::PrintMatrix(bool dirFlag) {
	if (dirFlag){
		Print(m_fwdtfm);
	} else {
		Print(m_bcktfm);
	}
}

void waRT::GTform::Print(const qbMatrix2<double> &matrix) {
	int nRows = matrix.GetNumRows();
	int nCols = matrix.GetNumCols();
	for (int row = 0; row<nRows; ++row) {
		for (int col = 0; col<nCols; ++col) {
			std::cout << std::fixed << std::setprecision(3) << matrix.GetElement(row, col) << " ";
		}
		std::cout << std::endl;
	}
}

void waRT::GTform::PrintVector(const qbVector<double> &inputVector) {
	int nRows = inputVector.GetNumDims();
	for (int row = 0; row < nRows; ++row) {
		std::cout << std::fixed << std::setprecision(3) << inputVector.GetElement(row) << std::endl;
	}
}