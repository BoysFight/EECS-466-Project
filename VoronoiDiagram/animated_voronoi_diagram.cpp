#include "animated_voronoi_diagram.h"
#include "GL/glut.h"

namespace voronoi_diagram
{
	void AnimatedVoronoiDiagram::display()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		float diagram_width = VoronoiDiagram::getWidth();
		float diagram_height = VoronoiDiagram::getHeight();
		gluOrtho2D(-diagram_width/2.0f, diagram_width/2.0f, -diagram_height/2.0f, diagram_height/2.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glPointSize(4.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_POINTS); {
			SitesConstPtr sites = VoronoiDiagram::sites_;
			for(Sites::const_iterator site = sites->begin();
				site != sites->end();
				site++)
			{
				glVertex2f((*site)->x, (*site)->y);
			}
		} glEnd();

		// draw a red line at the sweepline position
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINES); {
			if(curr_event_)
			{
				glVertex2f(-diagram_width/2.0f, curr_event_->y_);
				glVertex2f(diagram_width/2.0f, curr_event_->y_);
			}
		}; glEnd();


		glutSwapBuffers();
	}

	void AnimatedVoronoiDiagram::nextEvent()
	{
		if(event_queue_.empty())
			return;

		do {
			curr_event_ = event_queue_.top();
			event_queue_.pop();
			if(deleted_events_.find(curr_event_) != deleted_events_.end())
			{
				// remove from the set of deleted events
				deleted_events_.erase(curr_event_);
				curr_event_.reset();
				continue;
			}
		} while(!curr_event_);
	}

	void AnimatedVoronoiDiagram::restartAnimation()
	{
		edges_.reset(new Edges());
		while(!event_queue_.empty())
			event_queue_.pop();
		beachline_.reset();

		for(Sites::iterator site = sites_->begin();
			site != sites_->end();
			site++)
		{
			EventPtr site_event(new Event(*site));
			event_queue_.push(site_event);
		}
	}
}