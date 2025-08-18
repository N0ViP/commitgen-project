"""
Custom exceptions used by commitgen.
"""
class CommitGenError(Exception):
    """Base exception for commitgen."""
    pass

class GitRepoError(CommitGenError):
    """Raised when git repository issues occur."""
    pass

class NoStagedChangesError(CommitGenError):
    """Raised when no staged changes exist."""
    pass

