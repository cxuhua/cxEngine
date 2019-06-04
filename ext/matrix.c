/*
Copyright (c) 2008, Luke Benstead.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "matrix.h"
#include "mat4stack.h"

/* ---
 * Begin additions by Tobias Lensing for icedcoffee-framework.org */

typedef struct km_mat4_stack_context {
    km_mat4_stack modelview_matrix_stack;
    km_mat4_stack projection_matrix_stack;
    km_mat4_stack texture_matrix_stack;
    km_mat4_stack* current_stack;
    unsigned char initialized;
    void *contextRef;
    struct km_mat4_stack_context_list *entry;
} km_mat4_stack_context;

static km_mat4_stack_context *current_context = NULL;

void kmGLSetCurrentContext(void)
{
    if (current_context == NULL) {
        current_context = malloc(sizeof(km_mat4_stack_context));
        memset(current_context, 0, sizeof(km_mat4_stack_context));
    }
}

void kmGLClearContext(km_mat4_stack_context *context)
{
    /*Clear the matrix stacks*/
	km_mat4_stack_release(&context->modelview_matrix_stack);
	km_mat4_stack_release(&context->projection_matrix_stack);
	km_mat4_stack_release(&context->texture_matrix_stack);
    
	/*Delete the matrices*/
	context->initialized = 0;
    
    /*Set the current stack to point nowhere*/
	context->current_stack = NULL;
    
    /* Free the list entry, including its stacks*/
    free(context->entry);
    context->entry = NULL;
}

void kmGLClearCurrentContext(void)
{
    if(current_context != NULL){
        kmGLClearContext(current_context);
        current_context = NULL;
    }
}


/* End additions by Tobias Lensing for icedcoffee-framework.org
 * --- */

km_mat4_stack_context *lazyInitializeCurrentContext()
{
    assert(current_context != NULL && "No context set");
    
	if (current_context && !current_context->initialized) {
		kmMat4 identity; /*Temporary identity matrix*/

		/*Initialize all 3 stacks*/
		/*modelview_matrix_stack = (km_mat4_stack*) malloc(sizeof(km_mat4_stack));*/
		km_mat4_stack_initialize(&current_context->modelview_matrix_stack);

		/*projection_matrix_stack = (km_mat4_stack*) malloc(sizeof(km_mat4_stack));*/
		km_mat4_stack_initialize(&current_context->projection_matrix_stack);

		/*texture_matrix_stack = (km_mat4_stack*) malloc(sizeof(km_mat4_stack));*/
		km_mat4_stack_initialize(&current_context->texture_matrix_stack);

		current_context->current_stack = &current_context->modelview_matrix_stack;
		current_context->initialized = 1;

		kmMat4Identity(&identity);

		/*Make sure that each stack has the identity matrix*/
		km_mat4_stack_push(&current_context->modelview_matrix_stack, &identity);
		km_mat4_stack_push(&current_context->projection_matrix_stack, &identity);
		km_mat4_stack_push(&current_context->texture_matrix_stack, &identity);
	}
    
    return current_context;
}

void kmGLMatrixMode(kmGLEnum mode)
{
	km_mat4_stack_context *ctx = lazyInitializeCurrentContext();

	switch(mode)
	{
		case KM_GL_MODELVIEW:
			ctx->current_stack = &ctx->modelview_matrix_stack;
		break;
		case KM_GL_PROJECTION:
			ctx->current_stack = &ctx->projection_matrix_stack;
		break;
		case KM_GL_TEXTURE:
			ctx->current_stack = &ctx->texture_matrix_stack;
		break;
		default:
			assert(0 && "Invalid matrix mode specified"); /*TODO: Proper error handling*/
		break;
	}
}

void kmGLPushMatrix(void)
{
	kmMat4 top;

	km_mat4_stack_context *ctx = lazyInitializeCurrentContext();

	/*Duplicate the top of the stack (i.e the current matrix)	*/
	kmMat4Assign(&top, ctx->current_stack->top);
	km_mat4_stack_push(ctx->current_stack, &top);
}

void kmGLPopMatrix(void)
{
    assert(current_context->initialized && "Cannot Pop empty matrix stack");
	/*No need to lazy initialize, you shouldnt be popping first anyway!*/
	km_mat4_stack_pop(current_context->current_stack, NULL);
}

void kmGLLoadIdentity()
{
	km_mat4_stack_context *ctx = lazyInitializeCurrentContext();
	kmMat4Identity(ctx->current_stack->top); /*Replace the top matrix with the identity matrix*/
}

void kmGLMultMatrix(const kmMat4* pIn)
{
	km_mat4_stack_context *ctx = lazyInitializeCurrentContext();
	kmMat4Multiply(ctx->current_stack->top, current_context->current_stack->top, pIn);
}

void kmGLLoadMatrix(const kmMat4* pIn)
{
	km_mat4_stack_context *ctx = lazyInitializeCurrentContext();
	kmMat4Assign(ctx->current_stack->top, pIn);
}

void kmGLGetMatrix(kmGLEnum mode, kmMat4* pOut)
{
	km_mat4_stack_context *ctx = lazyInitializeCurrentContext();

	switch(mode)
	{
		case KM_GL_MODELVIEW:
			kmMat4Assign(pOut, ctx->modelview_matrix_stack.top);
		break;
		case KM_GL_PROJECTION:
			kmMat4Assign(pOut, ctx->projection_matrix_stack.top);
		break;
		case KM_GL_TEXTURE:
			kmMat4Assign(pOut, ctx->texture_matrix_stack.top);
		break;
		default:
			assert(1 && "Invalid matrix mode specified"); /*TODO: Proper error handling*/
		break;
	}
}

void kmGLTranslatef(float x, float y, float z)
{
	kmMat4 translation;

	/*Create a rotation matrix using the axis and the angle*/
	kmMat4Translation(&translation,x,y,z);

	/*Multiply the rotation matrix by the current matrix*/
	kmMat4Multiply(current_context->current_stack->top, current_context->current_stack->top, &translation);
}

void kmGLRotatef(float angle, float x, float y, float z)
{
	kmVec3 axis;
	kmMat4 rotation;

	/*Create an axis vector*/
	kmVec3Fill(&axis, x, y, z);

	/*Create a rotation matrix using the axis and the angle*/
	kmMat4RotationAxisAngle(&rotation, &axis, kmDegreesToRadians(angle));

	/*Multiply the rotation matrix by the current matrix*/
	kmMat4Multiply(current_context->current_stack->top, current_context->current_stack->top, &rotation);
}

void kmGLScalef(float x, float y, float z)
{
	kmMat4 scaling;
	kmMat4Scaling(&scaling, x, y, z);
	kmMat4Multiply(current_context->current_stack->top, current_context->current_stack->top, &scaling);
}
