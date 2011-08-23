//
//  PSPDFController.h
//  PSPDFKit
//
//  Created by Peter Steinberger on 7/20/11.
//  Copyright 2011 Peter Steinberger. All rights reserved.
//

#import "AQGridView.h"

@protocol PSPDFViewControllerDelegate;
@class PSPDFDocument, PSPDFScrollView, PSPDFScrobbleBar;

/// current active view mode
enum {
    PSPDFViewModeDocument,
    PSPDFViewModeThumbnails
}typedef PSPDFViewMode;

/// active page mode
enum {
    PSPDFPageModeSingle,
    PSPDFPageModeDouble,
    PSPDFPageModeAutomatic // single in portrait, double in landscape. Default.
}typedef PSPDFPageMode;

/// active scrolling direction
enum {
    PSPDFScrollingHorizontal, // default
    PSPDFScrollingVertical
}typedef PSPDFScrolling;

/// status bar style (old status will be restored regardless of the style chosen)
enum {
    PSPDFStatusBarInherit,      /// don't change status bar style, but show/hide statusbar on HUD events
    PSPDFStatusBarSmartBlack,   /// use UIStatusBarStyleBlackOpaque on iPad, UIStatusBarStyleBlackTranslucent on iPhone.
    PSPDFStatusBarBlackOpaque,  /// Opaque Black everywhere
    PSPDFStatusBarDefaultWhite, /// Switch to default (white) statusbar
    PSPDFStatusBarDisable       /// never show status bar
}typedef PSPDFStatusBarStyleSetting;

/// The main view controller to display pdfs. Can be displayed in fullscreen or embedded.
@interface PSPDFViewController : UIViewController <UIScrollViewDelegate, UIPopoverControllerDelegate, AQGridViewDelegate, AQGridViewDataSource> {
    id<PSPDFViewControllerDelegate> delegate_;
    PSPDFDocument *document_;
    AQGridView *gridView_;
    
    // view states
    UISegmentedControl *viewModeSegment_;
    NSUInteger lastPage_;
    NSUInteger page_;
    UIInterfaceOrientation lastOrientation_;
    
    // paging scrollview
    UIScrollView *pagingScrollView_;
    NSMutableSet *recycledPages_;
    NSMutableSet *visiblePages_;
    
    // for rotation event
    NSInteger targetPageAfterRotate_;
    
    // toolbar updating
    UIBarButtonItem *magazineButton_;
    UIToolbar *leftToolbar_;
    UIBarButtonItem *viewModeButton_;
    UIView *hudView_;
    
    UIPopoverController *popoverController_;
    PSPDFScrobbleBar *scrobbleBar_;
    PSPDFViewMode viewMode_;
    PSPDFPageMode pageMode_;
    PSPDFScrolling pageScrolling_;
    PSPDFStatusBarStyleSetting statusBarStyleSetting_;
    BOOL savedStatusBarVisibility_;
    UIStatusBarStyle savedStatusBarStyle_;
    UIColor *backgroundColor_;
    CGFloat iPhoneThumbnailSizeReductionFactor_;
    CGFloat pagePadding_;
    BOOL doublePageModeOnFirstPage_;
    BOOL navigationBarHidden_;
    BOOL scrobbleBarEnabled_;
    BOOL toolbarEnabled_;
    BOOL zoomingSmallDocumentsEnabled_;
    BOOL shadowEnabled_;
    BOOL scrollOnTapPageEndEnabled_;
    BOOL suppressHUDHideOnce_;
    BOOL magazineRectCacheLoaded_;
}

/// initialize with a document
- (id)initWithDocument:(PSPDFDocument *)document;

/// control currently displayed page
- (void)scrollToPage:(NSUInteger)page animated:(BOOL)animated;

/// control currently displayed page, optionally show/hide the HUD
- (void)scrollToPage:(NSUInteger)page animated:(BOOL)animated hideHUD:(BOOL)hideHUD;

/// scroll to next page
- (BOOL)scrollToNextPageAnimated:(BOOL)animated;

// scroll to previous page
- (BOOL)scrollToPreviousPageAnimated:(BOOL)animated;

/// depending on pageMode, this returns true if two pages are displayed
- (BOOL)isDualPageMode;

/// show a modal view controller with automatically added close button on the left side.
- (void)presentModalViewControllerWithCloseButton:(UIViewController *)controller;

/// reload scrollview. Call if you manually change the view width/height or some property inside PSPDFDocument. Usually not needed.
- (void)reloadData;

/// reload scrollview, scroll to specified page.
- (void)reloadDataAndScrollToPage:(NSUInteger)page;

/// checks if the current page is on the right side, when in double page mode. Page starts at 0.
- (BOOL)isRightPageInDoublePageNode:(NSUInteger)page;

/// register delegate to capture events, change properties
@property(nonatomic, assign) id<PSPDFViewControllerDelegate> delegate;

/// document that will be displayed.
@property(nonatomic, retain) PSPDFDocument *document;

/// current page displayed, not landscape corrected
@property(nonatomic, assign, readonly) NSUInteger page;

/// current page displayed, landscape corrected
@property(nonatomic, assign, readonly) NSUInteger realPage;

/// view mode: PSPDFViewModeMagazine or PSPDFViewModeThumbnails
@property(nonatomic, assign) PSPDFViewMode viewMode;

/// page mode: PSPDFPageModeSingle or PSPDFPageModeDouble
@property(nonatomic, assign) PSPDFPageMode pageMode;

/// change scrolling direction. defaults to horizontal scrolling (PSPDFScrollingHorizontal)
@property(nonatomic, assign) PSPDFScrolling pageScrolling;

/// shows first document page alone. Not relevant in PSPDFPageModeSinge. Defaults to NO.
@property(nonatomic, assign, getter=isDoublePageModeOnFirstPage) BOOL doublePageModeOnFirstPage;

/// allow zooming of small documents to screen width/height. Defaults to YES.
@property(nonatomic, assign, getter=isZoomingSmallDocumentsEnabled) BOOL zoomingSmallDocumentsEnabled;

/// page padding width between single/double pages. Defaults to 20.
@property(nonatomic, assign) CGFloat pagePadding;

/// enable/disable shadow
@property(nonatomic, assign, getter=isShadowEnabled) BOOL shadowEnabled;

/// saves the popoverController if currently displayed
@property(nonatomic, retain) UIPopoverController *popoverController;

/// if not set, we'll use scrollViewTexturedBackgroundColor as default.
@property(nonatomic, retain) UIColor *backgroundColor;

/// enables bottom scrobble bar [if HUD is displayed]. will be hidden automatically when in thumbnail mode. Defaults to YES. Set before loading view.
/// there's some more logic involved, e.g. is the default white statusbar not hidden on a HUD change.
@property(nonatomic, assign, getter=isSrobbleBarEnabled) BOOL scrobbleBarEnabled;

/// enables default header toolbar. Only displayed if inside UINavigationController. Defaults to YES. Set before loading view.
@property(nonatomic, assign, getter=isToolbarEnabled) BOOL toolbarEnabled;

/// status bar styling. Defaults to PSPDFStatusBarSmartBlack.
@property(nonatomic, assign) PSPDFStatusBarStyleSetting statusBarStyleSetting;

/// tap on begin/end of page scrolls to previous/next page. Defaults to YES.
@property(nonatomic, assign, getter=isScrollOnTapPageEndEnabled) BOOL scrollOnTapPageEndEnabled;

/// thumbnails on iPhone are smaller - you may change the reduction factor. Defaults to 0.588
@property(nonatomic, assign) CGFloat iPhoneThumbnailSizeReductionFactor;

/// view that is displayed as HUD. Make a KVO on viewMode if you build a different HUD for thumbnails view.
@property(nonatomic, retain, readonly) UIView *hudView;

/// show or hide HUD controls, titlebar, status bar (iPhone only)
@property(nonatomic, assign, getter=isHUDVisible) BOOL HUDVisible;

/// animated show or hide HUD controls, titlebar, status bar (iPhone only)
- (void)setHUDVisible:(BOOL)show animated:(BOOL)animated;

// ****** functions you may wanna override in subclasses ****

/// if you override PSPDFScrollView, change this.
- (Class)scrollViewClass;

/// override if you're changing the toolbar to your own
/// note that the toolbar is only displayed, if PSPDFViewController is inside a UINavigationController!
- (void)createToolbar;
- (UIBarButtonItem *)toolbarBackButton; // defaults to "Documents"
- (NSArray *)additionalLeftToolbarButtons; // not used when not modal
- (void)updateToolbars;

// called from scrollviews
- (void)showControls;
- (void)hideControls;
- (void)toggleControls;
- (NSUInteger)landscapePage:(NSUInteger)aPage;

@end
