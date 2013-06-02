SVM_readme


Library Usage
=============

These functions and structures are declared in the header file `svm.h`.
You need to #include "svm.h" in your C/C++ source files and link your
program with `svm.cpp`. You can see `svm-train.c` and `svm-predict.c`
for examples showing how to use them.

Before you classify test data, you need to construct an SVM model
(`svm_model`) using training data. A model can also be saved in
a file for later use. Once an SVM model is available, you can use it
to classify new data.

- Function: struct svm_model *svm_train(const struct svm_problem *prob,
					const struct svm_parameter *param);

    This function constructs and returns an SVM model according to
    the given training data and parameters.

    struct svm_problem describes the problem:
	
	struct svm_problem
	{
		int l;
		double *y;
		struct svm_node **x;
	};
 
    where `l` is the number of training data, and `y` is an array containing
    their target values. (integers in classification, real numbers in
    regression) `x` is an array of pointers, each of which points to a sparse
    representation (array of svm_node) of one training vector.

    For example, if we have the following training data:

    LABEL	ATTR1	ATTR2	ATTR3	ATTR4	ATTR5
    -----	-----	-----	-----	-----	-----
      1		  0 	0.1	  	0.2	  	0	  	0
      2		  0	  	0.1	  	0.3	 	-1.2	0
      1		  0.4	0	  	0	  	0	  	0
      2		  0 	0.1	  	0 		1.4		0.5
      3		 -0.1 	-0.2 	0.1 	1.1 	0.1

    then the components of svm_problem are:

    l = 5

    y -> 1 2 1 2 3

    x -> [ ] -> (2,0.1) (3,0.2) (-1,?)
	 [ ] -> (2,0.1) (3,0.3) (4,-1.2) (-1,?)
	 [ ] -> (1,0.4) (-1,?)
	 [ ] -> (2,0.1) (4,1.4) (5,0.5) (-1,?)
	 [ ] -> (1,-0.1) (2,-0.2) (3,0.1) (4,1.1) (5,0.1) (-1,?)

    where (index,value) is stored in the structure `svm_node`:

	struct svm_node
	{
		int index;
		double value;
	};

    index = -1 indicates the end of one vector.
 
    struct svm_parameter describes the parameters of an SVM model:

	struct svm_parameter
	{
		int svm_type;
		int kernel_type;
		double degree;	// for poly
		double gamma;	// for poly/rbf/sigmoid
		double coef0;	// for poly/sigmoid

		// these are for training only
		double cache_size; // in MB
		double eps;	// stopping criteria
		double C;	// for C_SVC, EPSILON_SVR, and NU_SVR
		int nr_weight;		// for C_SVC
		int *weight_label;	// for C_SVC
		double* weight;		// for C_SVC
		double nu;	// for NU_SVC, ONE_CLASS, and NU_SVR
		double p;	// for EPSILON_SVR
		int shrinking;	// use the shrinking heuristics
	};

    svm_type can be one of C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR.

    C_SVC:		C-SVM classification
    NU_SVC:		nu-SVM classification
    ONE_CLASS:		one-class-SVM
    EPSILON_SVR:	epsilon-SVM regression
    NU_SVR:		nu-SVM regression

    kernel_type can be one of LINEAR, POLY, RBF, SIGMOID.

    LINEAR:	u'*v
    POLY:	(gamma*u'*v + coef0)^degree
    RBF:	exp(-gamma*|u-v|^2)
    SIGMOID:	tanh(gamma*u'*v + coef0)

    cache_size is the size of the kernel cache, specified in megabytes.
    C is the cost of constraints violation. (we usually use 1 to 1000)
    eps is the stopping criterion. (we usually use 0.00001 in nu-SVC,
    0.001 in others). nu is the parameter in nu-SVM, nu-SVR, and
    one-class-SVM. p is the epsilon in epsilon-insensitive loss function
    of epsilon-SVM regression. shrinking = 1 means shrinking is conducted;
    = 0 otherwise.

    nr_weight, weight_label, and weight are used to change the penalty
    for some classes (If the weight for a class is not changed, it is
    set to 1). This is useful for training classifier using unbalanced
    input data or with asymmetric misclassification cost.

    nr_weight is the number of elements in the array weight_label and
    weight. Each weight[i] corresponds to weight_label[i], meaning that
    the penalty of class weight_label[i] is scaled by a factor of weight[i].
    
    If you do not want to change penalty for any of the classes,
    just set nr_weight to 0.

    *NOTE* Because svm_model contains pointers to svm_problem, you can
    not free the memory used by svm_problem if you are still using the
    svm_model produced by svm_train().

- Function: double svm_predict(const struct svm_model *model,
                             const struct svm_node *x);

    This function does classification or regression on a test vector x
    given a model.

    For a classification model, the predicted class for x is returned.
    For a regression model, the function value of x calculated using
    the model is returned. For one-class model, +1 or -1 is returned.

- Function: int svm_save_model(const char *model_file_name,
			       const struct svm_model *model);

    This function saves a model to a file; returns 0 on success, or -1
    if an error occurs.

- Function: struct svm_model *svm_load_model(const char *model_file_name);

    This function returns a pointer to the model read from the file,
    or a null pointer if the model could not be loaded.


- Function: void svm_destroy_model(struct svm_model *model);

    This function frees the memory used by a model.
